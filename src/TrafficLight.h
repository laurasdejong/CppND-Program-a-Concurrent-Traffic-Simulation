#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;

enum TrafficLightPhase { red, green };


// FP.3 Define a class „MessageQueue“ which has the public methods send and receive. check
// Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type. check
// Also, the class should define an std::dequeue called _queue, which stores objects of type TrafficLightPhase.check
// Also, there should be an std::condition_variable as well as an std::mutex as private members. check

template <class T>
class MessageQueue
{
public:
    void send(T &&msg);
    T receive();


private:
    std::deque<TrafficLightPhase> _queue; //in assignment 3, I think it should be deque<T>
    std::condition_variable _condition;
    std::mutex _mutex;

};

// FP.1 : Define a class „TrafficLight“ which is a child class of TrafficObject. check
// The class shall have the public methods „void waitForGreen()“ and „void simulate()“ check
// as well as „TrafficLightPhase getCurrentPhase()“, where TrafficLightPhase is an enum that check
// can be either „red“ or „green“. Also, add the private method „void cycleThroughPhases()“. check
// Furthermore, there shall be the private member _currentPhase which can take „red“ or „green“ as its value. check

class TrafficLight: public TrafficObject
{
public:
    // constructor / desctructor
    TrafficLight();

    // getters / setters

    // typical behaviour methods
    void waitForGreen();
    void simulate();
    TrafficLightPhase getCurrentPhase();

private:

    // typical behaviour methods
    void cycleThroughPhases();
    TrafficLightPhase _currentPhase;

    // FP.4b : create a private member of type MessageQueue for messages of type TrafficLightPhase 
    // and use it within the infinite loop to push each new TrafficLightPhase into it by calling 
    // send in conjunction with move semantics.
    MessageQueue<TrafficLightPhase> _messages;

    std::condition_variable _condition;
    std::mutex _mutex;
};

#endif