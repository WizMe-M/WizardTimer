#ifndef Timer_h
#define Timer_h

#include <Arduino.h>
#include <inttypes.h>

#define FOREVER -1

enum TimerStates
{
    Run = 0,
    Pause = 1,
    Stop = 2
};

class Timer
{
public:
    Timer(void)
    {
        state = Stop;
        wasPaused = false;
    }

    // Инициализация конечного таймера
    void init(unsigned long period, void (*callback)(), int repeatCount = FOREVER)
    {
        state = Stop;
        this->period = period;
        this->pauseInterval = period;
        this->callback = callback;
        this->repeatCount = repeatCount;
        currentCount = 0;
        wasPaused = false;
    }

    // Обновление таймера, вызывать в `loop`
    void update(void)
    {
        if (state == Run && millis() - lastMillis >= pauseInterval)
        {
            if (wasPaused)
            {
                pauseInterval = period;
                wasPaused = false;
            }

            (*callback)();
            lastMillis = millis();
            currentCount++;

            if (repeatCount > FOREVER && currentCount >= repeatCount)
            {
                stop();
            }
        }
    }

    // Запуск/возобновление таймера
    void start(void)
    {
        state = Run;
        lastMillis = millis();
    }

    // Постановка таймера на паузу
    void pause(void)
    {
        if (state == Run)
        {
            state = Pause;
            pauseInterval = period;// - (millis() - lastMillis); wtf
            wasPaused = true;
        }
    }

    // Полная остановка таймера
    void stop(void)
    {
        state = Stop;
        currentCount = 0;
        pauseInterval = period;
        wasPaused = false;
    }

private:
    // Текущее состояние таймера
    TimerStates state;

    // Время с последнего тика таймера
    unsigned long lastMillis;

    // Был ли таймер поставлен на паузу
    bool wasPaused;

    // Остаток времени до следующего тика таймера после остановки
    unsigned long pauseInterval;

    // Время до следующего тика таймера
    unsigned long period;

    // Количество всех тиков
    int repeatCount;

    // Текущее количество тиков (обнуляется при старте таймера)
    int currentCount;

    // Подписанная на тик таймера процедура
    void (*callback)(void);
};
#endif