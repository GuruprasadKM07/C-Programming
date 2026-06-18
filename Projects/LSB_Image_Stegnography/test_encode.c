#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include "common.h"
int main(int argc, char *argv[])
{
    if(argc >= 3 && argc <= 5)
    {
	if(check_operation_type(argv) == e_encode)
	{
	    // Before encoding creat structure variable
	    EncodeInfo encInfo;
	    // Enoding part begins
	    if(read_and_validate_encode_args(argv, &encInfo) == e_success)
	    {
		printf("INFO: Validation Successfull \n");
		sleep(1);
		if(do_encoding(&encInfo) == e_success)
		{
			printf("INFO: ######## Encoding Done Successfull ########\n");		
		}
		else
		{
		    printf("Encoding failure\n");
		    return 0;
		}
	    }
	    else
	    {
		printf("Validation failure\n");
		return 0;
	    }

	}
	else if(check_operation_type(argv) == e_decode)
	{
	    //Before decoding create structure variable
	    DecodeInfo decInfo;

	    //read and validate input args
	    if(read_and_validate_decode_args(argv, &decInfo) == e_success)
	    {
		printf("INFO: Validation successfull\n");

		if(do_decoding(&decInfo) == e_success)
		{
		    printf("####### Decoding Done Successfull ########\n");
		}
		else
		{
		    printf("Decoding failure\n");
		    return 0;
		}
	    }
	    else
	    {
		printf("Pls pass valid input\n");
		return 0;
	    }
	}
	else
	{
	    printf("Please pass corret option\n");
	    return 0;
	}
    }
    else
    {
	printf("Please pass correct number of arguments \n");
	return 0;
    }
    return 0;
}

OperationType check_operation_type(char *argv[])
{
    printf("INFO: Validation Process Started\n");
    sleep(1);
    if(!strcmp(argv[1],"-e"))
	return e_encode;
    else if(!strcmp(argv[1],"-d"))
	return e_decode;
    else
	return e_unsupported;
}
