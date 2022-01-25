#include <iostream>
#include <random>
#include "TrafficLight.h"
#include <thread>

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait()  check
    // to wait for and receive new messages and pull them from the queue using move semantics. check
    // The received object should then be returned by the receive function. 

    std::unique_lock<std::mutex> uLock(_mutex);
        _condition.wait(uLock, [this] { return !_queue.empty(); }); // pass unique lock to condition variable

        // remove last vector element from queue
        T msg = std::move(_queue.back());
        _queue.pop_back();

        return msg; // w


}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> check
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification. check


    // perform vector modification under the lock
    std::lock_guard<std::mutex> uLock(_mutex);

    // add vector to queue
    std::cout << "   Message " << msg << " has been sent to the queue" << std::endl;
    _queue.push_back(std::move(msg));
    _condition.notify_one(); // notify client

    //     std::deque<TrafficLightPhase> _queue;
    // std::condition_variable _condition;
    // std::mutex _mutex;

}


/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. check
    // Once it receives TrafficLightPhase::green, the method returns. check
    while (true)
    {
        TrafficLightPhase message = _messages.receive();
        if (message == TrafficLightPhase::green){
            std::cout << "green!" << std::endl;
            return;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    std::cout << "Startup TrafficLights with number of threads" << threads.size()<<std::endl;
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases,this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles check
    // and toggles the current phase of the traffic light between red and green check
    // and sends an update method to the message queue using move semantics.
    // The cycle duration should be a random value between 4 and 6 seconds. check
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. check

    clock_t t_start;
    clock_t t_end;
    int duration;
    int cycle_duration;

    while (true){ //infinite loop
        t_start = clock();

        // Change color
        if (_currentPhase == TrafficLightPhase::red){
            _currentPhase = TrafficLightPhase::green;
        } else {
            _currentPhase = TrafficLightPhase::red;
        }

        //Send update
        // wss fout, want move currenPhase, dus dan is die leeg de tweede loop. Wss de if els vervangen door een vlag? wel llekker lui...
        _messages.send(std::move(_currentPhase));

        // set duration between 4 and 6 seconds
        cycle_duration = rand()%2000+4000;
        t_end = clock();

        // wait till cycle_duration
        duration = (t_end-t_start)/ (CLOCKS_PER_SEC/1000); //in ms
        if (duration < cycle_duration){
            std::this_thread::sleep_for(std::chrono::seconds(cycle_duration-duration));

        }

        // wait between cycles
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
