#ifndef CONFIG_QUERY_H
#define CONFIG_QUERY_H

int config_query(char *filename, char *section, char *name, char *value);
int config_update(char *filename, char *section, char *name, char *value);

int config_query_machine_sn(char *value);

#define MACHINE_SN_FILENAME	"config.dat"
#define MACHINE_SN_SECTION	"machine"
#define MACHINE_SN_NAME		"sn"

#define CONFIG_fILE "/flash/exec/config.dat"

#endif