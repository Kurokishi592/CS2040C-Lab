#include "queue_simulator.h"

/*
* Simulate Queue Logic
* Objective: Get service time of all customers in queue
*            so that for analysis, we can get waiting time = service time - arrival time.
* Logic:
* 1. Check the Priority Policy:
*    a. Customers by Arrival Time (FIFO)
*    b. Customers by Least Processing Time First (LPTF)
* 2. Insert the customers into a heap for Priority Queue ADT
* 3. Serve the customers and update each customer's service time = time after prev's customer's processing time
*    This means we need a "clock" to track time
* 4. Add Customers served into a new vector<Customer>, which is the queue with updated service times
* 
* 
*   Cannot use MaxHeap from heap.hpp, because Heap<Customer> does not define operator< for heapify, error will occur
*   I must use std::priority_queue :(
*   For a priority queue ADT where smaller Processing Time has higher priority
*   operator> : a customer is "greater" if his time is shorter
*   Hence, max here refers to customer with least processing time == highest priority
*/ 

/*
*   Takes in a vector<customer> type: a vector of `customer` objects
*   Returns a vector<customer> of the queue with each customer's expected service time
*/


vector<Customer> QueueSimulator::simulateQueue(const vector<Customer>& customers) {
    if (customers.size() == 0) throw std::out_of_range("No customers");

    vector<Customer> customers_served;
    Heap<Customer> queue;
    vector<Customer> buffer;
    vector<int> server_times(_num_servers, 0);

    // Init queue based on priority order
    for (size_t i = 0; i < customers.size(); i++) {
        customers[i].set_compare_by_processing_time(_priority_order);
        queue.insert(customers[i]);
    }

    // Main loop to serve customer
    while (customers_served.size() != customers.size()) {

        // Find the server with the minimum service time 
        int min_time_server = 0;
        for (int i = 0; i < _num_servers; i++) {
            if (server_times[i] < server_times[min_time_server]) min_time_server = i;
        }

        // Find next customer to queue
        bool found_customer = false;
        while (!queue.empty() && !found_customer) {
            Customer current_customer = queue.extractMax();
            // Check if current server is available
            if (current_customer.arrival_time() <= server_times[min_time_server]) {
                found_customer = true;
                current_customer.set_service_time(server_times[min_time_server]);
                server_times[min_time_server] = current_customer.service_time() + current_customer.processing_time();
                customers_served.push_back(current_customer);
            }
            else buffer.push_back(current_customer); //add current customer to buffer
        }

        // Increment time if customer not found
        if (!found_customer) server_times[min_time_server]++;

        // add buffer back to the queue, and clear buffer
        while (!buffer.empty()) {
            queue.insert(buffer.back());
            buffer.pop_back();
        }
    }
    return customers_served;
}


//#include <queue>
//
//vector<Customer> QueueSimulator::simulateQueue(const vector<Customer>& customers) {
//    // TODO: Implement this function.
//
//    int currentTime = 0; // this is my "clock" so that i can update the time members
//    int nCust = customers.size();
//    vector<Customer> customersServed;
//
//    vector<int> serverClock(_num_servers, 0); // initialise all server times to 0
//
//    // Set comparison operator> mode first
//    if (_priority_order) {
//        // sort by LPTF
//        Customer::set_compare_by_processing_time(true);
//    }
//    else {
//        // Sort by FIFO
//        Customer::set_compare_by_processing_time(false);
//    }
//
//    // Customer class only has Customer::operator>
//    // Create a lambda function so that we can compare operator< to store in priority queue
//    auto comparator = [this](const Customer& c1, const Customer& c2) {
//        if (_priority_order) {
//            // Compare by processing time (LPTF)
//            return c1.processing_time() > c2.processing_time();
//        }
//        else {
//            // Compare by arrival time (FIFO)
//            return c1.arrival_time() > c2.arrival_time();
//        }
//        };
//
//    // std::priority_queue<T, Container, Comparator> defaults as maxheap
//    std::priority_queue<Customer, vector<Customer>, decltype(comparator)> queue(comparator);
//
//
//    // first customer, compute the new current time after serving that fella
//    // at this new current time, see who have arrived, and serve the one with the least procesing time
//    int i = 0;
//    if (_priority_order) {
//        while (i < nCust || !queue.empty()) {
//            // push everyoe within the current time into PQ, which then auto sorts based on processing time
//            while (i < nCust && customers[i].arrival_time() <= currentTime) {
//                queue.push(customers[i]);
//                i++;
//            }
//
//            if (queue.empty()) {
//                // If no customers are available, advance time to the next customer's arrival
//                currentTime = customers[i].arrival_time();
//                Customer currCustomer = customers[i++];
//                currCustomer.set_service_time(currentTime);
//                currentTime += currCustomer.processing_time();
//                customersServed.push_back(currCustomer);
//            }
//            else {
//                // Serve the customer with the highest priority
//                // extractMax()
//                Customer currCustomer = queue.top();
//                queue.pop();
//                currCustomer.set_service_time(currentTime);
//                currentTime += currCustomer.processing_time();
//                customersServed.push_back(currCustomer);
//            }
//        }
//    }
//    else {
//        // implement FIFO 
//        for (const Customer& customer : customers) {
//            if (_num_servers == 1) {
//                if (currentTime < customer.arrival_time()) {
//                    currentTime = customer.arrival_time();
//                }
//                Customer currCustomer = customer;
//                currCustomer.set_service_time(currentTime);
//                currentTime += currCustomer.processing_time();
//                customersServed.push_back(currCustomer);
//            }
//            else {
//                // for multiple servers
//                auto next_server = std::min_element(serverClock.begin(), serverClock.end());
//                int server_index = std::distance(serverClock.begin(), next_server);
//
//                int service_start_time = std::max(*next_server, customer.arrival_time());
//
//                Customer currCustomer = customer;
//                currCustomer.set_service_time(service_start_time);
//                *next_server = service_start_time + currCustomer.processing_time();
//
//                customersServed.push_back(currCustomer);
//            }
//        }
//    }
//
//    return customersServed;
//}


//#include <queue>
//
//vector<Customer> QueueSimulator::simulateQueue(const vector<Customer>& customers) {
//    // TODO: Implement this function.
//
//    int currentTime = 0; // this is my "clock" so that i can update the time members
//    int nCust = customers.size();
//    vector<Customer> customersServed;
//
//    // Set comparison operator> mode first
//    if (_priority_order) {
//        // sort by LPTF
//        Customer::set_compare_by_processing_time(true);
//    }
//    else {
//        // Sort by FIFO
//        Customer::set_compare_by_processing_time(false);
//    }
//
//    // Customer class only has Customer::operator>
//    // Create a lambda function so that we can compare operator< to store in priority queue
//    auto comparator = [this](const Customer& c1, const Customer& c2) {
//        if (_priority_order) {
//            // Compare by processing time (LPTF)
//            return c1.processing_time() > c2.processing_time();
//        }
//        else {
//            // Compare by arrival time (FIFO)
//            return c1.arrival_time() > c2.arrival_time();
//        }
//        };
//
//    // std::priority_queue<T, Container, Comparator> defaults as maxheap
//    std::priority_queue<Customer, vector<Customer>, decltype(comparator)> queue(comparator);
//
//
//    // first customer, compute the new current time after serving that fella
//    // at this new current time, see who have arrived, and serve the one with the least procesing time
//    int i = 0;
//    while (i < nCust || !queue.empty()) {
//        // push everyoe within the current time into PQ, which then auto sorts based on processing time
//        while (i < nCust && customers[i].arrival_time() <= currentTime) {
//            queue.push(customers[i]);
//            i++;
//        }
//
//        if (queue.empty()) {
//            // If no customers are available, advance time to the next customer's arrival
//            currentTime = customers[i].arrival_time();
//            Customer currCustomer = customers[i++];
//            currCustomer.set_service_time(currentTime);
//            currentTime += currCustomer.processing_time();
//            customersServed.push_back(currCustomer);
//        }
//        else {
//            // Serve the customer with the highest priority
//            // extractMax()
//            Customer currCustomer = queue.top();
//            queue.pop();
//            currCustomer.set_service_time(currentTime);
//            currentTime += currCustomer.processing_time();
//            customersServed.push_back(currCustomer);
//        }
//    }
//
//    return customersServed;
//}




//vector<Customer> QueueSimulator::simulateQueue(const vector<Customer>& customers) {
//    // TODO: Implement this function.
//
//    int currentTime = 0; // this is my "clock" so that i can update the time members
//    vector<Customer> customersServed;
//
//    // Set comparison operator> mode first
//    if (_priority_order) {
//        // sort by LPTF
//        Customer::set_compare_by_processing_time(true);
//    }
//    else {
//        // Sort by FIFO
//        Customer::set_compare_by_processing_time(false);
//    }
//
//    // Customer class only has Customer::operator>
//    // Create a lambda function so that we can compare customers bidirectionally to store in priority queue
//    auto comparator = [this](const Customer& c1, const Customer& c2) {
//        if (_priority_order) {
//            // Compare by processing time (LPTF)
//            return c1.processing_time() > c2.processing_time();
//        }
//        else {
//            // Compare by arrival time (FIFO)
//            return c1.arrival_time() > c2.arrival_time();
//        }
//        };
//
//    // std::priority_queue<T, Container, Comparator> defaults as maxheap
//    std::priority_queue<Customer, vector<Customer>, decltype(comparator)> queue(comparator);
//
//    // Store customers in the priority queue
//    for (const Customer& customer : customers) {
//        queue.push(customer); // insertItem()
//    }
//
//    // Serve the customers
//    while (!queue.empty()) {
//        // extractMax()
//        Customer currentCustomer = queue.top();
//        queue.pop();
//
//        // Set current time to be at least the customer's arrival time
//        if (currentTime < currentCustomer.arrival_time()) {
//            currentTime = currentCustomer.arrival_time();
//        }
//
//        // Record the service time of the customer
//        currentCustomer.set_service_time(currentTime);
//        currentTime += currentCustomer.processing_time();
//
//        // Add this customer to to the back of the list of served customers
//        customersServed.push_back(currentCustomer);
//    }
//
//    return customersServed;
//}





 // Simulation stuff
 //     int nCust = customers.size();
 //     int totalWaitingTime = 0;
//int WT = 0; / waiting time for each customer
//cout << endl << endl << "Setup" << endl;
//cout << "List of customers and their arrival and processing times" << endl;
//for (i = 0; i < n_cust; i++)
//{
    //custList[i].setAT(i);
    //custList[i].setPT((n_cust - i) % (n_cust / 2) + 1 + (i % 2) * (n_cust / 2));
    //cout << "Customer " << i << " will arrive at time " << custList[i].AT() << " with PT=" << custList[i].PT() << endl;
//}
//cout << endl;
//for (int round = 0; round < 2; round++) {
//    // you may need a big modification within this for-loop
//    cout << endl << endl;
//    cout << "Test Round " << round + 1 << endl;
//    cout << (round == 0 ? "First come first serve" : "Customer with the least PT served first") << endl;
//    comparisonWay = round;
//    current_time = 0;
//    totalWaitingTime = 0;
//    queue.insert(custList[0]);
//
//    cout << "Customer arrives at time " << custList[0].AT() << " with PT=" << custList[0].PT() << endl;
//
//    while (!queue.empty()) {
//        // you should change all of the code here inside
//        Customer c = queue.extractMax();
//        cout << "Customer arrives when no one is waiting" << endl;
//        cout << "Customer is served at time " << current_time << " with AT=" << c.AT()
//            << " and PT=" << c.PT() << " after waiting for “
//            << WT << " min." << endl;
//    }
//    cout << "Total Waiting Time: " << totalWaitingTime << endl;
//    cout << "Average Waiting Time: " << totalWaitingTime / (float)n_cust << endl;
