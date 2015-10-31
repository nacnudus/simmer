#include <Rcpp.h>

#include "entity.h"
#include "simulator.h"

using namespace Rcpp;

//[[Rcpp::export]]
SEXP Simulator__new(SEXP name_, SEXP verbose_) {
  std::string name = as<std::string>(name_);
  bool verbose = as<bool>(verbose_);
  
  XPtr<Simulator> ptr(new Simulator(name, verbose), false); // What does this "false" mean?
  return ptr;
}

//[[Rcpp::export]]
void reset_(SEXP sim_) {
  XPtr<Simulator> sim(sim_);
  
  sim->reset();
}

//[[Rcpp::export]]
double now_(SEXP sim_) {
  XPtr<Simulator> sim(sim_);
  
  return sim->now();
}

//[[Rcpp::export]]
double peek_(SEXP sim_) {
  XPtr<Simulator> sim(sim_);
  
  return sim->peek();
}

//[[Rcpp::export]]
void step_(SEXP sim_) {
  XPtr<Simulator> sim(sim_);
  
  sim->step();
}

//[[Rcpp::export]]
void run_(SEXP sim_, SEXP until_) {
  XPtr<Simulator> sim(sim_);
  double until = as<double>(until_);
  
  sim->run(until);
}

//[[Rcpp::export]]
void add_generator_(SEXP sim_, SEXP name_prefix_, SEXP first_activity_, SEXP dist_, SEXP mon_) {
  XPtr<Simulator> sim(sim_);
  std::string name_prefix = as<std::string>(name_prefix_);
  Environment first_activity(first_activity_);
  Function dist(dist_);
  bool mon = as<bool>(mon_);
  
  sim->add_generator(name_prefix, first_activity, dist, mon);
}

//[[Rcpp::export]]
void add_resource_(SEXP sim_, SEXP name_, SEXP capacity_, SEXP queue_size_, SEXP mon_) {
  XPtr<Simulator> sim(sim_);
  std::string name = as<std::string>(name_);
  int capacity = as<int>(capacity_);
  int queue_size = as<int>(queue_size_);
  bool mon = as<bool>(mon_);
  
  sim->add_resource(name, capacity, queue_size, mon);
}

//[[Rcpp::export]]
SEXP get_mon_arrivals_(SEXP sim_) {
  XPtr<Simulator> sim(sim_);
  
  ArrStats* stats = sim->get_mon_arrivals();
  
  return Rcpp::List::create(Rcpp::Named("name") = stats->name,
                            Rcpp::Named("start_time") = stats->start_time,
                            Rcpp::Named("end_time") = stats->end_time,
                            Rcpp::Named("activity_time") = stats->activity_time,
                            Rcpp::Named("finished") = stats->finished);
}

//[[Rcpp::export]]
SEXP get_mon_resource_(SEXP sim_, SEXP name_) {
  XPtr<Simulator> sim(sim_);
  std::string name = as<std::string>(name_);
  
  ResStats* stats = sim->get_resource(name)->get_observations();
  
  return Rcpp::List::create(Rcpp::Named("time") = stats->time,
                            Rcpp::Named("server") = stats->server,
                            Rcpp::Named("queue") = stats->queue);
}

//[[Rcpp::export]]
int get_res_capacity_(SEXP sim_, SEXP name_){
  XPtr<Simulator> sim(sim_);
  std::string name = as<std::string>(name_);
  
  return sim->get_resource(name)->get_capacity();
}

//[[Rcpp::export]]
int get_res_queue_size_(SEXP sim_, SEXP name_){
  XPtr<Simulator> sim(sim_);
  std::string name = as<std::string>(name_);
  
  return sim->get_resource(name)->get_queue_size();
}

//[[Rcpp::export]]
int seize_(SEXP name_, SEXP arrival_, SEXP amount_) {
  std::string name = as<std::string>(name_);
  Arrival* arrival = (Arrival *)as<long int>(arrival_);
  int amount = as<int>(amount_);
  
  return arrival->sim->get_resource(name)->seize(arrival, amount);
}

//[[Rcpp::export]]
int release_(SEXP name_, SEXP arrival_, SEXP amount_) {
  std::string name = as<std::string>(name_);
  Arrival* arrival = (Arrival *)as<long int>(arrival_);
  int amount = as<int>(amount_);
  
  return arrival->sim->get_resource(name)->release(arrival, amount);
}
