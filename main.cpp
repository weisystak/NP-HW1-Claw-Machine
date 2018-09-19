#include <fstream>
#include <string>
#include <queue>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <iostream>
#include <algorithm>
#include <condition_variable>
#include <set>
#include <sstream>
#include <utility>

using namespace std;
using out = ostringstream; 

struct Custom
{

    inline bool operator< (const Custom& rhs) const
    {
        if(arrive_time != rhs.arrive_time)
            return arrive_time < rhs.arrive_time;
        
        return id < rhs.id;
    }


    int id;
    int arrive_time, continuous_round, rest_time, needed_rounds;
};


atomic<int> G;
vector<atomic<int>> machine_time(2);

std::condition_variable cv;
mutex queue_mutex, time_mutex, ans_mutex;

vector<pair<int,string>> ans;

set<Custom> custom_queue;

inline void lock_time(int me, int other)
{
    unique_lock<mutex> time_lock(time_mutex);
    cv.wait(
                time_lock, 
                [&me, &other]{ return machine_time[me] <= machine_time[other]; }
            );
}



void run_machine(int me)
{
    Custom cur_custom;
    int other = (me == 0) ? 1: 0;

    string str;
    int t;

    string yes_no;

    for(;;)
    {
        {
            lock_guard<mutex> queue_lock(queue_mutex);
            if(custom_queue.empty())
            {
                cout<<"exit thread "<<me<<endl;
                machine_time[me] = INT32_MAX;
                break;
            }
            else
            {
                cur_custom = *custom_queue.begin();
                custom_queue.erase(custom_queue.begin());
            }
        }
        
        
        // it has no-ops interval
        if(machine_time[me] < cur_custom.arrive_time )
        {
            machine_time[me] = cur_custom.arrive_time; 
        }
        // it has waiting customer
        else if(machine_time[me] > cur_custom.arrive_time)
        {
            {
                lock_guard<mutex> lk(ans_mutex);
                t = cur_custom.arrive_time;
                out o;
                o<<cur_custom.arrive_time<<" "<<cur_custom.id<<" "<<"wait in line"<<" #"<<me+1;
                ans.push_back(make_pair(t, o.str()));
            }
        }
             

        cv.notify_one();
        lock_time(me, other);
        
        {
            lock_guard<mutex> lk(ans_mutex);
            t = machine_time[me];
            out o;
            o<<machine_time[me]<<" "<<cur_custom.id<<" "<<"start playing"<<" #"<<me+1;
            ans.push_back(make_pair(t, o.str()));
        }

        // can finish
        if(cur_custom.needed_rounds <= cur_custom.continuous_round)  
        {
            machine_time[me] += cur_custom.needed_rounds;
            cur_custom.needed_rounds = 0;

            yes_no = "YES";
        }
        // can't finish
        else
        {
            machine_time[me] += cur_custom.continuous_round;
            cur_custom.needed_rounds -= cur_custom.continuous_round;
            cur_custom.arrive_time += cur_custom.rest_time;

            yes_no = "NO";
            {
                lock_guard<mutex> queue_lock(queue_mutex);
                custom_queue.insert(cur_custom);
            }
            
        }


        
        
        cv.notify_one();
        lock_time(me, other);

        if(cur_custom.needed_rounds == 0)
        {
            lock_guard<mutex> lk(ans_mutex);
            t = machine_time[me];
            out o;
            o<<machine_time[me]<<" "<<cur_custom.id<<" "<<"finish playing "<<yes_no<<" #"<<me+1;
            ans.push_back(make_pair(t, o.str()));
        }
    }
    
}

int main(int argc, char *argv[])
{   
    for(int i = 1; i<argc; i++)
    {

        ifstream fin(argv[i]);
        machine_time[0] = 0;
        machine_time[1] = 0;

        custom_queue.clear();
        ans.clear();

        int G, n;
        fin>>G>>n;

        string str;
        Custom c;
        int id = 1;
        
        for(int j = 0; j<n; j++)
        {   
            c.id = id++;
            fin>>c.arrive_time>>c.continuous_round>>c.rest_time>>c.needed_rounds;
            custom_queue.insert(c);
        }
        
        thread a(run_machine, 0);
        thread b(run_machine, 1);

        if(a.joinable())
            a.join();
        if(b.joinable())
            b.join();

        sort(ans.begin(), ans.end());
        for(auto& x : ans)
        {
            cout<<x.second<<endl;
        }

        cout<<endl;
        cout<<"***************************************************************"<<endl;
    }
}