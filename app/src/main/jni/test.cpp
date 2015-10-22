#include <jni.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>
#include <ctype.h>

extern "C" {
#include <codec.h>
}

#include <android/log.h>

#define RLOG(args...) __android_log_print(ANDROID_LOG_VERBOSE, "core", args)

codec_para_t pcodec;
FILE* fp = NULL;
bool thread_exit = false;
bool forward = false;
bool backward = false;

static int set_avsync_enable(int enable)
{
    int fd;
    const char *path = "/sys/class/tsync/enable";
    char  bcmd[16];
    fd = open(path, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd >= 0) {
        sprintf(bcmd, "%d", enable);
        write(fd, bcmd, strlen(bcmd));
        close(fd);
        return 0;
    }

    return -1;
}

int set_sys_int(const char *path,int val)
{
    int fd;
    char  bcmd[16];
    fd=open(path, O_CREAT|O_RDWR | O_TRUNC, 0644);
    if(fd>=0)
    {
        sprintf(bcmd,"%d",val);
        write(fd,bcmd,strlen(bcmd));
        RLOG("set fs%s=%d ok\n",path,val);
        close(fd);
        return 0;
    }
    RLOG("set fs %s=%d failed\n",path,val);
    return -1;
}

#define READ_SIZE 64*1024

int start_play(const char *filename)
{
    char buffer[READ_SIZE];
    buf_status bstat;

    codec_audio_basic_init();

    memset(&pcodec, 0, sizeof(codec_para_t ));

    pcodec.has_video = 1;
    pcodec.video_pid = 0x100;
    pcodec.video_type = VFORMAT_H264;

    pcodec.has_audio = 1;
    pcodec.audio_pid = 0x101;
    pcodec.audio_type = AFORMAT_AAC;
    pcodec.stream_type = STREAM_TYPE_TS;

    set_sys_int("/sys/class/video/blackout_policy", 0);

    if((fp = fopen(filename,"rb")) == NULL)
    {
        RLOG("open file error!\n");
        return -1;
    }

    int ret = codec_init(&pcodec);
    if(ret != CODEC_ERROR_NONE)
    {
        RLOG("codec init failed, ret=-0x%x", -ret);
        return -1;
    }

    set_avsync_enable(1);

    while(!thread_exit)
    {
        if (backward) {
            RLOG("Resetting codec\n");
            codec_reset(&pcodec);
            forward = backward = false;
        }

        int len = fread(buffer, 1, READ_SIZE, fp);

        if ( len <= 0 )
        {
            RLOG("Resetting codec\n");
            codec_reset(&pcodec);
            rewind(fp);
            continue;
        }

        int isize = 0;
        do {
            ret = codec_write(&pcodec, buffer + isize, len);
            if (ret < 0) {
                if (errno != EAGAIN) {
                    RLOG("write data failed, errno %d\n", errno);
                    goto error;
                }
                else {
                    continue;
                }
            }
            else {
                isize += ret;
            }
        } while(isize < len);

        do {
            ret = codec_get_vbuf_state(&pcodec, &bstat);
            if (ret != 0) {
                RLOG("codec_get_vbuf_state error: %x\n", -ret);
                goto error;
            }
            usleep(1000);
        } while (bstat.data_len > 30*1024);
    }
error:
    codec_close(&pcodec);
    fclose(fp);

    return 0;
}

extern "C" JNIEXPORT jint JNICALL Java_com_aplhakill_app_MainActivity_copen
        (JNIEnv *env, jobject thisObj, jstring url) {

    const char *nurl = (*env).GetStringUTFChars(url, 0);

    start_play(nurl);

    (*env).ReleaseStringUTFChars(url, nurl);
    return 0;
}

extern "C" JNIEXPORT jint JNICALL Java_com_aplhakill_app_MainActivity_cforward
        (JNIEnv *env, jobject thisObj) {
    forward = true;
    return 0;
}

extern "C" JNIEXPORT jint JNICALL Java_com_aplhakill_app_MainActivity_cbackward
        (JNIEnv *env, jobject thisObj) {
    backward = true;
    return 0;
}

extern "C" JNIEXPORT jint JNICALL Java_com_aplhakill_app_MainActivity_cclose
        (JNIEnv *env, jobject thisObj) {
    thread_exit = true;
    return 0;
}