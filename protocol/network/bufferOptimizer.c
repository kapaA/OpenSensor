#include "network.h"



GLOB_RET kbBufOptimizer(frame *f, unsigned long readTimeA)
{
    GLOB_RET ret = GLOB_FAILURE;
    frame tempFrame;
    char s[96];
    unsigned long readTime=0;
    int i = 0;
    int fromAcnt = 0, fromBcnt = 0;
    int rmFrom = 0;
    int fromSrc = (int) f->srcAddr;
    int bufferSizeHalf = BUFFER_SIZE/2;

    while(i<BUFFER_SIZE) //Checks the buffer for packets
    {
        ret = get_frame(&tempFrame, &readTime ,i, &keepBufPar);
        if(GLOB_SUCCESS == ret)
        {
            if(0x01 == tempFrame.payload.isncHeader.originalSrcAddr) fromAcnt++;
            else if (0x02 == tempFrame.payload.isncHeader.originalSrcAddr) fromBcnt++;
        }
        i++;
    }

    if(fromAcnt > bufferSizeHalf) rmFrom = 1;
    else if(fromBcnt > bufferSizeHalf) rmFrom = 2;

    Log.inKeepBufCntA = fromAcnt;
    Log.inKeepBufCntB = fromBcnt;
    //if (fromSrc == rmFrom || fromAcnt == fromBcnt) CHANGE! 31-07-13
    if(abs(fromAcnt - fromBcnt) <= 1)
    {
        if(0x01 == tempFrame.payload.isncHeader.originalSrcAddr) Log.keepBufDropA++;
        else if (0x02 == tempFrame.payload.isncHeader.originalSrcAddr) Log.keepBufDropB++;
        return KEEP_BUFFER_IGNORE;
    }
    else
    {
        i = 0;
        do
        {
            ret = get_frame(&tempFrame, &readTime ,i, &keepBufPar);
            if(rmFrom == (int)tempFrame.payload.isncHeader.originalSrcAddr)
            {
                if (GLOB_SUCCESS == ret)
                {
                    ret = remove_frame(i, &keepBufPar); //OFF INTERREST!
                    if(1 == rmFrom ) Log.keepBufDropA++;
                    else if(2 == rmFrom) Log.keepBufDropB++;
                }
                else
                {
                    sprintf(s,"LOG_ERROR 1\n");
                    output_serial(s, sizeof(s));
                }
                if(GLOB_SUCCESS == ret)
                {
                    //if(1 == rmFrom ) Log.inBufDropA++; CHANGE! 31-07-13
                    //else if(2 == rmFrom) Log.inBufDropB++;
                    //if(1 == rmFrom ) Log.keepBufDropA++;
                    //else if(2 == rmFrom) Log.keepBufDropB++;
                    keepBufPar.timeSem = 0x01;
                    if(!buffer_full(&keepBufPar)) //CHANGE!
                    {
                        write_buffer(f,&keepBufPar,&readTimeA);
                    }
                    keepBufPar.timeSem = 0x00;
                    return ret;
                }
                else
                {
                    keepBufPar.timeSem = 0x00;
                }
            }

            i++;
        }while(i<keepBufPar.data_size);
    }
    return ret;
}
