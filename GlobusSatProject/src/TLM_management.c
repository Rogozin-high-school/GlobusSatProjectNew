/*
 * filesystem.c
 *
 *  Created on: 20 ֳ¡ֳ®ֳ¸ֳµ 2019
 *      Author: Idan
 */


#include <satellite-subsystems/GomEPS.h>
#include <hal/Timing/Time.h>
#include <hcc/api_fat.h>
#include <hal/errors.h>
#include <hcc/api_hcc_mem.h>
#include <string.h>
#include <hcc/api_mdriver_atmel_mcipdc.h>
#include <hal/Storage/FRAM.h>
#include <at91/utility/trace.h>
#include "TLM_management.h"
#include <stdlib.h>
#include <GlobalStandards.h>

#define SKIP_FILE_TIME_SEC 1000000
#define _SD_CARD 0
#define FIRST_TIME -1
#define FILE_NAME_WITH_INDEX_SIZE MAX_F_FILE_NAME_SIZE+sizeof(int)*2

//struct for filesystem info
typedef struct
{
	int num_of_files;
} FS;
//TODO remove all 'PLZNORESTART' from code!!
#define PLZNORESTART() gom_eps_hk_basic_t myEpsTelemetry_hk_basic;	(GomEpsGetHkData_basic(0, &myEpsTelemetry_hk_basic)); //todo delete

//struct for chain file info
typedef struct //מבנה של שרשרת אחת
{
	int size_of_element;
	char name[FILE_NAME_WITH_INDEX_SIZE]; //מערך במקום מצביע למערך
	unsigned int creation_time;//זמן יצירה
	unsigned int last_time_modified;
	int num_of_files;

} C_FILE;
#define C_FILES_BASE_ADDR (FSFRAM+sizeof(FS))


void delete_allTMFilesFromSD()
{
}
// return -1 for FRAM fail
static int getNumOfFilesInFS()//output how many files by this struct
{
	FS fs;
	if (FRAM_read((unsigned int *)&fs,FSFRAM,sizeof(FS))!=0)
	{
		return -1;
	}
	return fs.num_of_files;
}
//return -1 on fail
static int setNumOfFilesInFS(int new_num_of_files)
{
	FS fs;
	fs.num_of_files=new_num_of_files;

	if(FRAM_write((unsigned char*)&fs,FSFRAM,sizeof(FS))!=0) //כשלון הכתיבה לפראם
	 	{
			return -1;
		}
		return 0;
}
FileSystemResult InitializeFS(Boolean first_time)
{
	int ret;
		hcc_mem_init(); /* Initialize the memory to be used by filesystem */

		ret = fs_init(); /* Initialize the filesystem */
		if(ret != F_NO_ERROR )
		{
			TRACE_ERROR("fs_init pb: %d\n\r", ret);
			return FS_FAT_API_FAIL;
		}

		ret = f_enterFS(); /* Register this task with filesystem */
		if(ret != F_NO_ERROR )
		{
			TRACE_ERROR("f_enterFS pb: %d\n\r", ret);
			return FS_FAT_API_FAIL;
		}

		ret = f_initvolume( 0, atmel_mcipdc_initfunc, _SD_CARD ); /* Initialize volID as safe */

		if( F_ERR_NOTFORMATTED == ret )
		{
			TRACE_ERROR("Filesystem not formated!\n\r");
			return FS_FAT_API_FAIL;
		}
		else if( F_NO_ERROR != ret)
		{
			TRACE_ERROR("f_initvolume pb: %d\n\r", ret);
			return FS_FAT_API_FAIL;
		}
		if(first_time)
		{
			delete_allTMFilesFromSD();
			FS fs = {0};
			if(FRAM_write((unsigned char*)&fs,FSFRAM,sizeof(FS))!=0)
			{
				return FS_FAT_API_FAIL;
			}
		}

		F_SPACE space;
		/* get free space on current drive */
		ret = f_getfreespace(f_getdrive(),&space);
		if(!ret)
		{
		printf("There are %lu bytes total, %lu bytes free, \
		%lu bytes used, %lu bytes bad.",
		space.total, space.free, space.used, space.bad);
		}
		else
		{
		printf("\nError %d reading drive\n", ret);
		}

		return FS_SUCCSESS;
}

//only register the chain, files will create dynamically
FileSystemResult c_fileCreate(char* c_file_name,
		int size_of_element)
{
	return FS_SUCCSESS;
}
//write element with timestamp to file
static void writewithEpochtime(F_FILE* file, byte* data, int size,unsigned int time)
{
}
// get C_FILE struct from FRAM by name
static Boolean get_C_FILE_struct(char* name,C_FILE* c_file,unsigned int *address)
{

//	PLZNORESTART();//macro for dont showing in 5 minuts

	int i;
	unsigned int c_file_address = 0;
	int err_read=0;
	int num_of_files_in_FS = getNumOfFilesInFS();
	for(i =0; i < num_of_files_in_FS; i++)			//search correct c_file struct
	{
		c_file_address= C_FILES_BASE_ADDR+sizeof(C_FILE)*(i);
		err_read = FRAM_read((unsigned char*)c_file,c_file_address,sizeof(C_FILE));
		if(0 != err_read)
		{
			printf("FRAM error in 'get_C_FILE_struct()' error = %d\n",err_read);
			return FALSE;
		}

		if(!strcmp(c_file->name,name))//if equals return 0
		{
			if(address != NULL)//if the user ask for the address
			{
				*address = c_file_address;
			}
			return TRUE;//stop when found
		}
	}
	return FALSE;
}//address include the add. for the c_file that we wanted.
//calculate index of file in chain file by time
static int getFileIndex(unsigned int creation_time, unsigned int current_time)
{
	return 0;
}
//write to curr_file_name
void get_file_name_by_index(char* c_file_name,int index,char* curr_file_name)
{
}
FileSystemResult c_fileReset(char* c_file_name)
{
	return FS_SUCCSESS;
}
FileSystemResult c_fileWrite(char* c_file_name, void* element)
{
	return FS_SUCCSESS;
}
FileSystemResult fileWrite(char* file_name, void* element,int size)
{

	F_FILE *file;
	unsigned int curr_time; //unix time how many time pass since 1/1/1970
	int err = Time_getUnixEpoch(&curr_time);
	if(err != 0)
	{
		return FS_FAIL;
	}
	file= f_open(file_name,"a");
	 if (!file)
	 {
		 int rc=f_getlasterror();
		 if(rc == F_ERR_TOOLONGNAME)
		 {
			 return FS_TOO_LONG_NAME;
		 }
		 if(rc == F_ERR_ALLOCATION)
		 {
			 return FS_ALLOCATION_ERROR;
		 }
		 printf ("f_open failed, errorcode:%d\n",rc);
		 return rc;
	 }
	if(f_write(&curr_time,sizeof(int),1,file)!=1)
	{
		f_flush(file);
		f_close(file);
		return FS_FAIL;
	}
	if(f_write(element,1,size,file)!=size)
	{
		f_flush(file);
		f_close(file);
		return FS_FAIL;
	}
	f_flush(file);
	f_close(file);
	return FS_SUCCSESS;

}
static FileSystemResult deleteElementsFromFile(char* file_name,unsigned long from_time,
		unsigned long to_time,int full_element_size)
{
	return FS_SUCCSESS;
}
FileSystemResult c_fileDeleteElements(char* c_file_name, time_unix from_time,
		time_unix to_time)
{
	return FS_SUCCSESS;
}
FileSystemResult fileRead(char* c_file_name,byte* buffer, int size_of_buffer,
		time_unix from_time, time_unix to_time, int* read, int element_size)
{
	*read=0;
		F_FILE* current_file= f_open(c_file_name,"r");
		int buffer_index = 0;
		void* element;
		unsigned int size_elementWithTimeStamp = element_size+sizeof(unsigned int);
		element = malloc(size_elementWithTimeStamp);//store element and his timestamp
		unsigned int length =f_filelength(c_file_name)/(size_elementWithTimeStamp);//number of elements in currnet_file
		int err_fread=0;

		//f_seek( current_file, 0L , SEEK_SET );
		for(unsigned int j=0;j < length;j++)
		{
			err_fread = f_read(element,(size_t)size_elementWithTimeStamp,(size_t)1,current_file);
			(void)err_fread;
			unsigned int element_time = *((unsigned int*)element);
			printf("read element, time is %u\n",element_time);
			if(element_time > to_time)
			{
				break;
			}

			if(element_time >= from_time)
			{
				if((unsigned int)buffer_index>(unsigned int)size_of_buffer)
				{
					free(element);
					return FS_BUFFER_OVERFLOW;
				}
				(*read)++;
				memcpy(buffer + buffer_index,element,size_elementWithTimeStamp);
				buffer_index += size_elementWithTimeStamp;
			}
		}
		f_close(current_file);


		free(element);

		return FS_SUCCSESS;
}
FileSystemResult c_fileRead(char* c_file_name,byte* buffer, int size_of_buffer,
		time_unix from_time, time_unix to_time, int* read,time_unix* last_read_time)
{
	return FS_SUCCSESS;
}
void print_file(char* c_file_name)
{
}

void DeInitializeFS( void )
{
}
typedef struct{
	int a;
	int b;
}TestStruct ;
