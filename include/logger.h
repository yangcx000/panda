#ifndef LOGGER_H
#define LOGGER_H

#include <stdbool.h>
#include <sys/syslog.h>

#define LOG_SPACE_SIZE (1 * 1024 * 1024)
#define LOG_SPACE_DEBUG_SIZE (32 * 1024 * 1024)
#define MAX_MSG_SIZE 1024
#define MAX_THREAD_NAME_LEN	20

extern int panda_log_level;

enum log_dst_type {
	LOG_DST_DEFAULT,
	LOG_DST_STDOUT,
	LOG_DST_SYSLOG,
};

int log_init(const char *progname, enum log_dst_type type, int level,
	     char *outfile);
void log_close(void);
void log_write(int prio, const char *func, int line, const char *fmt, ...)
	__printf(4, 5);
void set_thread_name(const char *name, bool show_idx);
void get_thread_name(char *name);

/* sheep log priorities, compliant with syslog spec */
#define	PANDA_EMERG	LOG_EMERG
#define	PANDA_ALERT	LOG_ALERT
#define	PANDA_CRIT	LOG_CRIT
#define	PANDA_ERR	LOG_ERR
#define	PANDA_WARNING	LOG_WARNING
#define	PANDA_NOTICE	LOG_NOTICE
#define	PANDA_INFO	LOG_INFO
#define	PANDA_DEBUG	LOG_DEBUG

#define panda_emerg(fmt, args...) \
	log_write(PANDA_EMERG, __func__, __LINE__, fmt, ##args)
#define panda_alert(fmt, args...) \
	log_write(PANDA_ALERT, __func__, __LINE__, fmt, ##args)
#define panda_crit(fmt, args...) \
	log_write(PANDA_CRIT, __func__, __LINE__, fmt, ##args)
#define panda_err(fmt, args...) \
	log_write(PANDA_ERR, __func__, __LINE__, fmt, ##args)
#define panda_warn(fmt, args...) \
	log_write(PANDA_WARNING, __func__, __LINE__, fmt, ##args)
#define panda_notice(fmt, args...) \
	log_write(PANDA_NOTICE, __func__, __LINE__, fmt, ##args)
#define panda_info(fmt, args...) \
	log_write(PANDA_INFO, __func__, __LINE__, fmt, ##args)

/*
 * 'args' must not contain an operation/function with a side-effect.  It won't
 * be evaluated when the log level is not SDOG_DEBUG.
 */
#define panda_debug(fmt, args...)						\
({									\
	if (unlikely(panda_log_level == PANDA_DEBUG))			\
		log_write(PANDA_DEBUG, __func__, __LINE__, fmt, ##args);	\
})

static inline int loglevel_str2num(const char *str)
{
	static const char * const loglevel_table[] = {
		"emerg",
		"alert",
		"crit",
		"err",
		"warning",
		"notice",
		"info",
		"debug",
	};
	int i, max = ARRAY_SIZE(loglevel_table);

	for (i = 0; i < max; i++) {
		if (!strcmp(loglevel_table[i], str))
			break;
	}

	return i == max ? -1 : i;
}

void set_loglevel(int new_loglevel);
int get_loglevel(void);

extern pid_t logger_pid;

#endif	/* LOG_H */
