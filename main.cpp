#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// Simple job structure
struct Job {
    int jobId;
    int arrivalDay;
    int arrivalHour;
    int coresRequired;
    int memoryRequired;
    int executionTime;
};

// Simple worker node structure
struct Worker {
    int id;
    int availableCores;    // starts with 24
    int availableMemory;   // starts with 64
    bool isBusy;
};

// Function to read jobs from file
vector<Job> readJobs(const string& filename) {
    vector<Job> jobs;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        Job job;
        istringstream readLine(line);
        string temp;

        // Format: JobId: X Arrival Day: X Time Hour: X MemReq: X CPUReg: X ExeTime: X
        readLine >> temp >> job.jobId;           // Read "JobId:" and the value
        readLine >> temp >> temp >> job.arrivalDay;    // Read "Arrival Day:" and the value
        readLine >> temp >> temp >> job.arrivalHour;   // Read "Time Hour:" and the value
        readLine >> temp >> job.memoryRequired;  // Read "MemReq:" and the value
        readLine >> temp >> job.coresRequired;   // Read "CPUReg:" and the value
        readLine >> temp >> job.executionTime;   // Read "ExeTime:" and the value

        jobs.push_back(job);
        cout << "Read job " << job.jobId << endl;
    }

    return jobs;
}

// Function to find first available worker for a job
int findAvailableWorker(vector<Worker>& workers, Job job) {
    for (int i = 0; i < workers.size(); i++) {
        if (!workers[i].isBusy &&
            workers[i].availableCores >= job.coresRequired &&
            workers[i].availableMemory >= job.memoryRequired) {
            return i;
        }
    }
    return -1;  // No worker found
}

int main() {
    // Initialize 128 workers
    vector<Worker> workers;
    for (int i = 0; i < 128; i++) {
        Worker w = {i, 24, 64, false};  // id, cores, memory, not busy
        workers.push_back(w);
    }

    // Read jobs from file
    vector<Job> jobs = readJobs("JobArrival.txt");
    cout << "\nTotal jobs read: " << jobs.size() << endl;

    //FCFS
    vector<Job> pendingJobs;  // Jobs that couldn't be allocated
    int allocatedJobs = 0;

    for (const Job& job : jobs) {
        int workerIndex = findAvailableWorker(workers, job);

        if (workerIndex != -1) {
            // Allocate job to worker
            workers[workerIndex].availableCores -= job.coresRequired;
            workers[workerIndex].availableMemory -= job.memoryRequired;
            workers[workerIndex].isBusy = true;
            allocatedJobs++;

            cout << "Job " << job.jobId << " allocated to worker " << workerIndex
                 << " (Cores used: " << job.coresRequired
                 << ", Memory used: " << job.memoryRequired << ")" << endl;
        } else {
            pendingJobs.push_back(job);
            cout << "Job " << job.jobId << " added to pending queue (no available workers)" << endl;
            return 1;//test   should be removed-------------------------------------------------------
        }
    }

    // Print summary
    cout << "\nScheduling Summary:" << endl;
    cout << "Total jobs: " << jobs.size() << endl;
    cout << "Jobs allocated: " << allocatedJobs << endl;
    cout << "Jobs pending: " << pendingJobs.size() << endl;

    return 0;
}


