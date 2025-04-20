// #include "queue_simulator.h"
// #include "heap.h"
#include <queue>
#include <vector>
#include <functional>
#include <iostream>

// class QueueSimulator 
// {
// private:
//     bool _priority_order; // Declare the member variable

// public:
vector<Customer> simulateQueue(const vector<Customer>& customers)
{

    std::vector<int> server_times(_num_servers, 0); // Initialize server times to 0

    // arrange based on processing time
    int current_time = 0;
    int total_customers = customers.size();
    vector<Customer> customers_served;

    // lambda function to sort customers by processing time
    auto compare_processing_time = [](const Customer& a, const Customer& b)
        {
            return a.processing_time() > b.processing_time();
        };

    // priority queue is default a max heap thus we have to convert it to a minheap via our lambda function and sort customers by processing time
    std::priority_queue<Customer, std::vector<Customer>, decltype(compare_processing_time)> PQ(compare_processing_time); // takes in type of data stored, vector of data, and comparator function and initialize to variable PQ


    if (_priority_order)
    {
        int i = 0;
        while (i < total_customers || !PQ.empty())
        {
            // Add customers who have arrived by current_time
            while (i < total_customers && customers[i].arrival_time() <= current_time)
            {
                PQ.push(customers[i]);
                i++;
            }

            // If the queue is empty, move current_time to the next customer's arrival
            if (PQ.empty())
            {
                current_time = customers[i].arrival_time();
                Customer current_customer = customers[i++];
                current_customer.set_service_time(current_time);
                current_time += current_customer.processing_time();
                customers_served.push_back(current_customer);
            }
            else
            {
                Customer current_customer = PQ.top();
                PQ.pop();
                current_customer.set_service_time(current_time);
                current_time += current_customer.processing_time();
                customers_served.push_back(current_customer);
            }
        }
    }
    // if priority order is false, process customers based on arrival time
    else // implement FIFO
    {
        // iterate through every customer in the queue
        for (const Customer& customer : customers)
        {
            if (_num_servers == 1)
            {
                if (current_time < customer.arrival_time())
                {
                    current_time = customer.arrival_time();
                }

                Customer current_customer = customer;
                current_customer.set_service_time(current_time);
                current_time += current_customer.processing_time();
                customers_served.push_back(current_customer);
            }
            else
            {
                // MULTI-SERVER: Assign customer to the next available server
                auto next_server = std::min_element(server_times.begin(), server_times.end());
                int server_index = std::distance(server_times.begin(), next_server);

                int service_start_time = std::max(*next_server, customer.arrival_time());

                Customer current_customer = customer;
                current_customer.set_service_time(service_start_time);
                *next_server = service_start_time + current_customer.processing_time();

                customers_served.push_back(current_customer);
            }
        }
    }
    return customers_served;
}
}