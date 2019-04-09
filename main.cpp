#include <iostream>
#include <filesystem>
#include <thread>
#include <future>

#include "Define.h"
#include "solver.h"
#include "HashGraphBuild.h"
#include "StandardGraphBuild.h"
#include "StandardGraphGoThrough.h"

//g++ -std=c++17 -Wall main.cpp -o ./cmake-build-debug/test -lstdc++fs --compile with gcc :)))

void solve(const char * filename, std::promise<std::string> *promise, int id){

    //TODO choose strategies
    auto *tsolver = new solver(id, filename, new HashGraphBuild(), new StandardGraphGoThrough());
    //std::cout<<"Solving"<<std::endl;
    tsolver->solve();
    //std::cout<<"Solving finished"<<std::endl;
    promise->set_value(tsolver->getResult());

    delete promise;
    delete tsolver;
}

void dummyTests(){
    Element e = Element("CTTAGCTA");
    std::cout << e.getValue() << std::endl;
    std::cout << e.getHead(3) << std::endl;
    std::cout << e.getHead(4) << std::endl;
    std::cout << e.getHead(5) << std::endl;
    std::cout << e.getHead(10) << std::endl;
    std::cout << e.getTail(3) << std::endl;
    std::cout << e.getTail(4) << std::endl;
    std::cout << e.getTail(5) << std::endl;
    std::cout << e.getTail(10) << std::endl;
    Element f = Element("TTAGCTAG");
    std::cout << e.appendSize(f) << std::endl;
    std::cout << e.getTail(7) << std::endl;
    std::cout << f.getHead(7) << std::endl;
    e.appendElement(f,7);

    std::cout << e.getValue() << std::endl;
    std::cout << e.getSize() << std::endl;
    std::cout << e.getParts()->size() <<std::endl;
    std::cout << e.getValue().size() <<std::endl;
    std::cout << e.calculateTotalLengthOfParts() << std::endl;
    Element g = Element("AGCCC");
    e.appendElement(g,2);
    std::cout << e.getValue() << std::endl;
    std::cout << e.getSize() << std::endl;
    std::cout << e.getParts()->size() <<std::endl;
    std::cout << e.getValue().size() <<std::endl;
    std::cout << e.calculateTotalLengthOfParts() << std::endl;
    Element h = Element("CCCCGAT");
    Element i = Element("CGATAAA");
    h.appendElement(i,4);
    e.appendElement(h,3);
    std::cout << e.getValue() << std::endl;
    std::cout << e.getSize() << std::endl;
    std::cout << e.getParts()->size() <<std::endl;
    std::cout << e.getValue().size() <<std::endl;
    std::cout << e.calculateTotalLengthOfParts() << std::endl;
    std::cout << "---" <<std::endl;
    std::cout << f.getValue() << std::endl;
    std::cout << f.getSize() <<std::endl;
    std::cout << f.calculateTotalLengthOfParts() << std::endl;
    std::cout << g.getValue() << std::endl;
    std::cout << g.getSize() <<std::endl;
    std::cout << g.calculateTotalLengthOfParts() << std::endl;
    std::cout << h.getValue() << std::endl;
    std::cout << h.getSize() <<std::endl;
    std::cout << h.calculateTotalLengthOfParts() << std::endl;
    std::cout << i.getValue() << std::endl;
    std::cout << i.getSize() <<std::endl;
    std::cout << i.calculateTotalLengthOfParts() << std::endl;

}

int main() {
    //dummyTests();

    std::string input_path = "../instances";

    // vector of filenames
    auto *filenames = new std::vector<std::string>();

    for(const auto & entry : std::filesystem::directory_iterator(input_path)){
        filenames->push_back(entry.path());
    }

    //number of instances
    size_t number_of_instances = filenames->size();

    //tab of threads - one per instance
    auto *threads = new std::thread[number_of_instances];
    //tab of futures - to get values from promises
    auto *futures = new std::future<std::string>[number_of_instances];
    //output vector
    auto *output = new std::vector<std::string>();

    //iterator for thread dispatch
    size_t iter = 0;

    //debug - sequential
    for(const auto & a : *filenames){
        auto *promise = new std::promise<std::string>();
        futures[iter] = promise->get_future();
        solve(a.c_str(), promise, iter);
        output->push_back(futures[iter].get());

        iter++;
    }

//    auto *promise = new std::promise<std::string>();
//    futures[iter] = promise->get_future();
//    solve(filenames->at(0).c_str(), promise, iter);
//    output->push_back(futures[iter].get());


//    //thread dispatch - max THREAD_NUM threads at once
//    for(const auto & a : *filenames){
//
//        //joining threads when it is to be more than THREAD_NUM of them at once
//        if(iter >= THREAD_NUM){
//            futures[iter-THREAD_NUM].wait();                    //wait for promise to complete
//            output->push_back(futures[iter-THREAD_NUM].get());  //get result from thread
//            threads[iter-THREAD_NUM].join();                    //join thread
//        }
//
//        //creating new threads
//        if(iter < number_of_instances){
//            auto *promise = new std::promise<std::string>();
//            futures[iter] = promise->get_future();
//            threads[iter] = std::thread(solve, a.c_str(), promise, iter);
//        }
//
//        iter++;
//    }
//
//    //close remaining threads
//    for(size_t i=iter-THREAD_NUM; i<iter; i++){
//        futures[i].wait();
//        output->push_back(futures[i].get());
//        threads[i].join();
//    }

    // save results -> to csv

    //open result file
    std::ofstream out;
    out.open("result.csv");

    //csv header line
    //std::cout<<solver::header_line;
    out << solver::header_line;

    for(const auto & o : *output){
        //std::cout << o << std::endl;
        //save to file
        out<<o;
    }

    //close file
    out.close();

    //--- deleting stuff ---//
    filenames->clear();
    filenames->shrink_to_fit();
    delete filenames;

    output->clear();
    output->shrink_to_fit();
    delete output;

    delete[] futures;
    delete[] threads;

    return 0;
}