#ifndef RULETIMEMANAGER_H
#define RULETIMEMANAGER_H

#include <pthread.h>
#include <mutex>
#include <util/Time.h>
#include <util/PriorityQueue.h>

class Rule;

/**
 * @brief The RuleTimerThread class is used to execute rules which contain a sleep action.
 * As threads should not be blocked by a sleep action, they add the rule conaining the sleep action to this thread,
 * which continues to execute them as soon as the time given by the sleep action has elapsed.
 */
class RuleTimerThread
{
public:
    RuleTimerThread();
    ~RuleTimerThread();

    void start();
    void kill();

    void clear();

    void addRule(Rule* rule, unsigned int start, unsigned int waitMs);

    void pauseTimer();
    void continueTimer();

private:
    struct Element
    {
        Rule* rule;
        unsigned int start;
        timespec time;

        bool operator< (const Element& rhs)
        {
            return timespecGreaterThan(this->time, rhs.time);
        }

        bool operator == (const Element& rhs)
        {
            // return true if rule objects match
            // this is the case if lhs and rhs are actually the same polling object
            return this->rule == rhs.rule;
        }
    };

    static void* run_internal(void* arg);
    void run();

    pthread_t m_thread;
    std::mutex m_mutex;
    bool m_bStop;
    PriorityQueue<Element> m_queue;
    bool m_bPaused;
    timespec m_pausedTime;
};

#endif // RULETIMEMANAGER_H
