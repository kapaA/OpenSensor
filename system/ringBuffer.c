#include "../commonVar.h"
#include "../commonDef.h"
#include "ringBuffer.h"


int buffer_full(bufPar *bPar)
{
    char s[96];
    if(bPar->data_size > BUFFER_SIZE )
    {
        sprintf(s,"LOG_ERROR 2\n");
        output_serial(s, sizeof(s));
    }
    else if(bPar->data_size < 0 )
    {
            sprintf(s,"LOG_ERROR 3\n");
    }
    if(bPar->read_pointer != bPar->write_pointer && bPar->data_size ==  BUFFER_SIZE) //CHANGE!
    {
        sprintf(s,"LOG_ERROR 4\n");
        bPar->read_pointer  = 0x00;
        bPar->data_size     = 0x00;
        bPar->write_pointer = 0x00;
    }

    if(bPar->data_size > BUFFER_SIZE)
    {
        sprintf(s,"LOG_ERROR 5\n");
    }
    return bPar->read_pointer == bPar->write_pointer &&
           bPar->data_size ==  BUFFER_SIZE;
}

int buffer_empty(bufPar *bPar) //Checks the buffer for contents
{
    char s[96];
    if(bPar->data_size > BUFFER_SIZE) //
    {
        sprintf(s,"LOG_ERROR 6\n");
        output_serial(s, sizeof(s));
    }
    else if(bPar->data_size < 0 )
    {
            sprintf(s,"LOG_ERROR 7\n");
            output_serial(s, sizeof(s));
    }
    if(bPar->read_pointer != bPar->write_pointer && bPar->data_size == 0) //CHANGE!
    {
            sprintf(s,"LOG_ERROR 8\n");
            output_serial(s, sizeof(s));
            
            bPar->read_pointer   = 0x00;
            bPar->data_size     = 0x00;
            bPar->write_pointer = 0x00;
    }
    
    if(bPar->data_size > BUFFER_SIZE) //
    {
        sprintf(s,"LOG_ERROR 9\n");
        output_serial(s, sizeof(s));
    }
    return bPar->read_pointer == bPar->write_pointer &&
           bPar->data_size == 0;
}

void write_buffer( frame *f, bufPar *bpar, unsigned long *readTime)
{
    char s[96];
    if(bpar->data_size > BUFFER_SIZE)
    {
        sprintf(s,"LOG_ERROR 10\n");
        output_serial(s, sizeof(s));
    }
    if (++bpar->write_pointer >= BUFFER_SIZE) bpar->write_pointer = 0;
    bpar->buffer[bpar->write_pointer] = *f;

    if(bpar->timeSem==0x01)
    {
        bpar->timerBuffer[bpar->write_pointer] = timer_ms;
    }else
    {
        bpar->timerBuffer[bpar->write_pointer] = *readTime;
    }

    if(bpar->data_size >= BUFFER_SIZE )
    {
       sprintf(s,"LOG_ERROR 11\n");
       output_serial(s, sizeof(s));
    }
    else
    {
        bpar->data_size++;
    }
}

void read_buffer(frame *f, bufPar *bPar, unsigned char opt, unsigned long *readTime)
{
    char s[96];
    if(bPar->data_size > BUFFER_SIZE)
    {
        sprintf(s,"LOG_ERROR 12\n");
        output_serial(s, sizeof(s));
    }
    if(!buffer_empty(bPar))
    {
        *readTime = bPar->timerBuffer[bPar->read_pointer+1];
        if(1==opt)
        {
            if (++bPar->read_pointer >= BUFFER_SIZE) bPar->read_pointer = 0;
            if(bPar->data_size <= 0)
            {
                
            }
            else
            {
                bPar->data_size--;
            }
            *f = bPar->buffer[bPar->read_pointer];
            bPar->buffer[bPar->read_pointer].destAddr = 0x00;
            bPar->buffer[bPar->read_pointer].srcAddr  = 0x00;
            bPar->buffer[bPar->read_pointer].type     = 0x00;
            bPar->buffer[bPar->read_pointer].seqNr    = 0x00;

            bPar->buffer[bPar->read_pointer].payload.isncHeader.codedPacket = 0x00;
            bPar->buffer[bPar->read_pointer].payload.isncHeader.firstSeqNr  = 0x00;
            bPar->buffer[bPar->read_pointer].payload.isncHeader.optimalCWmin_A = 0x00;
            bPar->buffer[bPar->read_pointer].payload.isncHeader.optimalCWmin_B = 0x00;

            bPar->buffer[bPar->read_pointer].payload.isncHeader.originalSrcAddr = 0x00;
            bPar->buffer[bPar->read_pointer].payload.isncHeader.rxTotal = 0x00;
            bPar->buffer[bPar->read_pointer].payload.isncHeader.secondDestAddr = 0x00;
            bPar->buffer[bPar->read_pointer].payload.isncHeader.secondSeqNr = 0x00;
            bPar->buffer[bPar->read_pointer].payload.isncHeader.testCaseType = 0x00;
            bPar->buffer[bPar->read_pointer].payload.isncHeader.txReq = 0x00;

            // resetting the packet palce holder
        }
        else if(2==opt)
        {
            *f = bPar->buffer[bPar->read_pointer+1];
        }
    }
    else
    {
 #if defined (DEBUG)
            putsUSBUSART("INVALID READ BUF!\n");
        #endif
    }
}
GLOB_RET remove_frame(int rmPointer, bufPar *bPar)
{
    char s[96];
    GLOB_RET ret = GLOB_ERROR_OUT_OF_RANGE_PARAM;
    char i = 0;

	if(bPar->write_pointer == 0 && bPar->read_pointer == 1  ) // Test
	{
		i = 0;
	}

	if(bPar->data_size > BUFFER_SIZE || bPar->write_pointer >= BUFFER_SIZE || bPar->read_pointer >= BUFFER_SIZE
		|| bPar->data_size < 0 || bPar->write_pointer < 0 || bPar->read_pointer < 0)
        {
            sprintf(s,"LOG_ERROR 13\n");
            output_serial(s, sizeof(s));
            ret = GLOB_ERROR_INVALID_PARAM;
            return ret;
        }
    if(((bPar->write_pointer > bPar->read_pointer) &&
       ((rmPointer <= bPar->write_pointer) && (rmPointer > bPar->read_pointer))) ||
       ((bPar->read_pointer > bPar->write_pointer) &&
       ((rmPointer > bPar->read_pointer) || (rmPointer <= bPar->write_pointer))) ||
       (BUFFER_SIZE == bPar->data_size))
        {
            while(rmPointer+i != bPar->write_pointer)
            {
                if(rmPointer+i+1 > BUFFER_SIZE-1)
                {
                    bPar->buffer[BUFFER_SIZE-1] = bPar->buffer[0];
                    bPar->timerBuffer[BUFFER_SIZE-1] = bPar->timerBuffer[0];
                    i = 0;
                    rmPointer = 0;
                }else
                {
                    bPar->buffer[rmPointer+i] = bPar->buffer[rmPointer+i+1];
                    bPar->timerBuffer[rmPointer+i] = bPar->timerBuffer[rmPointer+i+1];
                    i++;
                }
            }
			
			if(0 != bPar->write_pointer) //CHANGE!
			{
				bPar->write_pointer--;
				bPar->data_size--; 
			}
			else if (0 == bPar->write_pointer)
			{
				bPar->write_pointer = BUFFER_SIZE-1;
				bPar->data_size--; 
			}
			else
			{
				sprintf(s,"LOG_ERROR 14\n");
                                output_serial(s, sizeof(s));
			}
			ret = GLOB_SUCCESS;
        }
	else
	{
		ret = GLOB_WARNING_PARAM_NOT_SET;
		//putsUSBUSART("LOG_RME: %d %d %d wrd | rmP %d\n",bPar->write_pointer, bPar->read_pointer, bPar->data_size, rmPointer );
	}
    return ret;	
}

GLOB_RET get_frame(frame *f, unsigned long *time ,int rPointer, bufPar *bPar)
{
    GLOB_RET ret = GLOB_ERROR_OUT_OF_RANGE_PARAM;
    if(((bPar->write_pointer > bPar->read_pointer) &&
       ((rPointer <= bPar->write_pointer) && (rPointer > bPar->read_pointer))) ||
       ((bPar->read_pointer > bPar->write_pointer) &&
       ((rPointer > bPar->read_pointer) || (rPointer <= bPar->write_pointer))) ||
       (BUFFER_SIZE == bPar->data_size))
        {
            *f = bPar->buffer[rPointer];
            *time = bPar->timerBuffer[rPointer];
            ret = GLOB_SUCCESS;
        }
    return ret;
}

GLOB_RET read_time(unsigned long *time ,int rPointer, bufPar *bPar)
{
    GLOB_RET ret = GLOB_ERROR_OUT_OF_RANGE_PARAM;
    char i = 0;
    if(((bPar->write_pointer > bPar->read_pointer) &&
       ((rPointer <= bPar->write_pointer) && (rPointer > bPar->read_pointer))) ||
       ((bPar->read_pointer > bPar->write_pointer) &&
       ((rPointer > bPar->read_pointer) || (rPointer <= bPar->write_pointer))) ||
       (BUFFER_SIZE == bPar->data_size))
        {
            *time = bPar->timerBuffer[rPointer];
            ret = GLOB_SUCCESS;
        }
    return ret;
}








