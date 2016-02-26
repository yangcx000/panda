
void log_write(int prio, const char *func, int line, const char *fmt, ...)
{
    va_list ap;

    if (prio > sd_log_level)
        return;

    va_start(ap, fmt);
    dolog(prio, func, line, fmt, ap);
    va_end(ap);
}

void dolog(int prio, const char *func, int line, const char *fmt, va_list ap)
{
    char buf[sizeof(struct logmsg) + MAX_MSG_SIZE];
    char *str = buf + sizeof(struct logmsg);
    struct logmsg *msg = (struct logmsg *)buf;
    int len = 0;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    len = vsnprintf(str, MAX_MSG_SIZE, fmt, ap);
    if (len < 0) {
        syslog(LOG_ERR, "vsnprintf failed");
        return;
    }
    msg->str_len = min(len, MAX_MSG_SIZE - 1);

    char str_final[MAX_MSG_SIZE];

    init_logmsg(msg, &tv, prio, func, line);
    len = format->formatter(str_final, sizeof(str_final) - 1, msg, true);
    str_final[len++] = '\n';
    xwrite(fileno(stderr), str_final, len);
    fflush(stderr);
}
