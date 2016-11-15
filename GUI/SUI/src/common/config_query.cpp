#include "config_query.h"
#include "../common.h"
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//#define pret(x) (x)

bool noto_line(char *line)
{
	while( isspace( *line ) ) line++;
	
	return *line == '#';
}


bool section_line(char *line)
{
	while( isspace( *line ) ) line++;	
	if( *line != '[' ) return false;
	
	if( !(line = strchr(line, ']')) ) return false;
	line++;
	while( isspace( *line ) ) line++;
	
	return !line[0];
}


bool match_section(char *line, char *section)
{
	while( isspace( *line ) ) line++;	
	if( *line != '[' ) return false;
	
	int len = strlen( section );	
	if( strncmp(++line, section, len) ) return false;
	
	return line[ len ] == ']';
}

bool match_line(char *line, char *name)
{
	while( isspace( *line ) ) line++;
	
	while( *line == *name )
	{
		line++;
		name++;
	}
	
	return !name[0] && (isspace( *line ) || *line == '=');
}

void copy_line_value(char *str, char *value)
{
	strcpy(value, str);	
	char *tmp = value + (strlen(value) -1);
	
	while( isspace( *tmp ) && tmp >= value ) *tmp-- = '\0';
}

bool get_line_value(char *str, char *line, char *value)
{
	int len = strlen(line);
	
	while( isspace( *str ) ) str++;
	if( strncmp(str, line, len) ) return false;
	
	str += len;
	while( isspace( *str ) ) str++;
	
	if( *str++ != '=' ) return false;
	while( isspace( *str ) ) str++;
	
	copy_line_value(str, value);
	return true;
}


int config_query(char *filename, char *section, char *name, char *value)
{
	static char szvalue[256] = {""};

	char ptrline;
	int  iread;
	char szquery[1024];
	FILE *pf;
	
	if( !(pf = fopen(filename, "rb")) )
		return pret(-1);

	while( !feof(pf) )
	{
		if( !fgets(szquery, sizeof(szquery), pf) )
		{
			fclose(pf);
			return pret(-2);
		}
		
		if( match_section(szquery, section) ) break;
	}
	
	while( !feof(pf) )
	{
		if( !fgets(szquery, sizeof(szquery), pf) )
		{
			fclose(pf);
			return pret(-3);
		}
		
		if( section_line(szquery) ) break;
		
		if(  get_line_value(szquery, name, value) )
		{
			fclose(pf);
			return 0;
		}
	}

	fclose(pf);
	return pret(-4);
}

char* config_file_read(FILE *pf)
{
	char *ptr;
	int size;
	
	if( fseek(pf, 0, SEEK_END) )
		return 0;
	
	size = ftell(pf);
	ptr = (char*)malloc( size + 1 );
	
	if( !ptr ) return 0;
	
	if( fread(ptr, 1, size, pf) != size ) return 0;
	
	return ptr;
}

char *tmp_filename(char *filename)
{
	static char name[512];
	
	for(int i = 0; i < 10; i++)
	{
		sprintf(name, "%s.%d", filename, i);
		if( access(name, F_OK) ) return name;
	}
	
	return 0;
}

int touch_config_update(char *filename, char *section, char *name, char *value)
{
	FILE *pf = fopen(filename, "w");	
	if( !pf ) return pret(-1);
	
	fprintf(pf, "[%s]\n%s=%s\n", section, name, value);
	fclose(pf);
	
	return 0;
}

int section_config_update(FILE *pfin, FILE *pfout, char *name, char *value)
{
	bool putline = false;
	char szquery[1024];
	
	while( !feof(pfin) )
	{
		if( !fgets(szquery, sizeof(szquery), pfin) )
		{
			if( !putline ) fprintf(pfout, "%s=%s\n", name, value);
			break;
		}
		
		if( section_line(szquery) ) 
		{
			if( !putline ) fprintf(pfout, "%s=%s\n", name, value);
			if( fwrite(szquery, strlen(szquery), 1, pfout) != 1 ) return pret(-2);
			return 0;
		}
		
		if( match_line(szquery, name) )
		{
			fprintf(pfout, "%s=%s\n", name, value);
			putline = true;
			continue;
		}
		
		if( fwrite(szquery, strlen(szquery), 1, pfout) != 1 ) return pret(-3);
	}
	
	return 0;
}
	
int config_update(char *filename, char *section, char *name, char *value)
{
	char szquery[1024];
	FILE *pfin, *pfout;
	char *tmpname;
	bool lineputed = false;
	int ret = 0;
	
	if( access(filename, F_OK) ) return touch_config_update(filename, section, name, value);
	
	if( !(tmpname = tmp_filename(filename)) )
		return pret(-1);
	
	if( !(pfout = fopen(tmpname, "w")) )
		return pret(-2);
	
	if( !(pfin = fopen(filename, "r")) )
	{
		ret = -3;
		goto end;
	}
	
	while( !feof(pfin) )
	{
		if( !fgets(szquery, sizeof(szquery), pfin) )
			break;
		
		if( fwrite(szquery, strlen(szquery), 1, pfout) != 1 )
		{
			ret = -5;
			goto end;
		}
		
		if( match_section(szquery, section) )
		{
			if( section_config_update(pfin, pfout, name, value) ) 
			{
				ret = -6;
				goto end;
			}else
			{
				lineputed = true;
				continue;
			}
		}
	}

	if( !lineputed ) fprintf(pfout, "[%s]\n%s=%s\n", section, name, value);
end:
	fclose(pfin);
	fclose(pfout);
	if( ret )
	{
		remove(tmpname);
		return pret(ret);
	}
	remove(filename);
	rename(tmpname, filename);
	return 0;
}

int config_query_machine_sn(char *value)
{
	return config_query(MACHINE_SN_FILENAME, MACHINE_SN_SECTION, MACHINE_SN_NAME, value);
}

#if 0

int main(int argc, char *argv[])
{
	char value[128] = {""};
	if( argc >= 3 ) config_query(argv[1], argv[2], argv[3], value);
	
	printf("%s\n", value);
}

#endif
