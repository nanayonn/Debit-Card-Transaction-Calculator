#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 1
//

// Destructor
// TASK 1
//

// Overloaded < operator.
// TASK 2
//

// GIVEN
// Member functions to get values.
//

Transaction::Transaction(std::string ticker_symbol, unsigned int day_date,
                         unsigned int month_date, unsigned year_date,
                         bool buy_sell_trans, unsigned int number_shares,
                         double trans_amount)
{

  symbol = ticker_symbol;
  day = day_date;
  month = month_date;
  year = year_date;

  if (buy_sell_trans == true)
  {
    trans_type = "Buy";
  }
  if (buy_sell_trans == false)
  {
    trans_type = "Sell";
  }
  shares = number_shares;
  amount = trans_amount;
  trans_id = assigned_trans_id;
  assigned_trans_id = assigned_trans_id + 1;
  acb = 0;
  acb_per_share = 0;
  share_balance = 0;
  cgl = 0;
  p_next = nullptr;
}
bool Transaction::operator<(Transaction const &other)
{
  if (this->year < other.year)
  {
    return true;
  }
  if (this->month < other.month && this->year == other.year)
  {
    return true;
  }
  if (this->day < other.day && this->year == other.year && this->month == other.month)
  {
    return true;
  }
  return false;
}
Transaction::~Transaction()
{
}
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true : false; }
unsigned int Transaction::get_trans_id() const { return trans_id; }

Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb(double acb_value) { acb = acb_value; }
void Transaction::set_acb_per_share(double acb_share_value) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance(unsigned int bal) { share_balance = bal; }
void Transaction::set_cgl(double value) { cgl = value; }
void Transaction::set_next(Transaction *p_new_next) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print()
{
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
            << std::setw(4) << get_symbol() << " "
            << std::setw(4) << get_day() << " "
            << std::setw(4) << get_month() << " "
            << std::setw(4) << get_year() << " ";

  if (get_trans_type())
  {
    std::cout << "  Buy  ";
  }
  else
  {
    std::cout << "  Sell ";
  }

  std::cout << std::setw(4) << get_shares() << " "
            << std::setw(10) << get_amount() << " "
            << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
            << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
            << std::setw(10) << std::setprecision(3) << get_cgl()
            << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//

History::History()
{
  p_head = nullptr;
}
History::~History()
{
  Transaction *p_track{p_head};
  while (p_track != nullptr)
  {
    while (p_head != nullptr)
    {
      p_track = p_track->get_next();
      delete p_head;
      p_head = p_track;
    }
  }
};
void History::read_history()
{
  ece150::open_file();
  while (ece150::next_trans_entry() == true)
  {
    Transaction *p_node = new Transaction(ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), ece150::get_trans_year(), ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount());
    insert(p_node);
  }
  ece150::close_file();
};
void History::insert(Transaction *p_new_trans)
{
  if (p_head == nullptr)
  {
    p_head = p_new_trans;
  }
  else
  {
    Transaction *p_tem{p_head};
    while (p_tem->get_next() != nullptr)
    {
      p_tem = p_tem->get_next();
    }
    p_tem->set_next(p_new_trans);
    p_tem = nullptr;
  }
};
void History::sort_by_date()
{
  if (p_head == nullptr || p_head->get_next() == nullptr)
  {
    return;
  }

  Transaction *p_sort{p_head};
  Transaction *p_tem{nullptr};
  p_head = p_head->get_next();
  p_sort->set_next(nullptr);
  while (p_head != nullptr)
  {
    p_tem = p_head;
    p_head = p_head->get_next();
    p_tem->set_next(nullptr);
    if (p_sort == nullptr)
    {
      p_sort = p_tem;
      p_tem = nullptr;
    }
    if (*p_tem < *p_sort)
    {
      p_tem->set_next(p_sort);
      p_sort = p_tem;
    }
    else
    {
      Transaction *p_insert{p_sort};
      while (p_insert->get_next() != nullptr && *p_insert->get_next() < *p_tem)
      {
        p_insert = p_insert->get_next();
      }
      p_tem->set_next(p_insert->get_next());
      p_insert->set_next(p_tem);
      p_tem=nullptr;
      p_insert=nullptr;
    }
  }
  p_head = p_sort;

  Transaction *p_sortb{p_head};
  Transaction *p_temb{nullptr};
  p_head = p_head->get_next();
  p_sortb->set_next(nullptr);
  while (p_head != nullptr)
  {
    p_temb = p_head;
    p_head = p_head->get_next();
    p_temb->set_next(nullptr);

    if (((p_temb->get_year()) == (p_sortb->get_year())) &&
        ((p_temb->get_month()) == (p_sortb->get_month())) &&
        ((p_temb->get_day()) == (p_sortb->get_day())))
    {
      if ((p_sortb->get_trans_id()) > (p_temb->get_trans_id()))
      {
        p_temb->set_next(p_sortb);
        p_sortb = p_temb;
      }
    }
    else
    {
      Transaction *p_insertb{p_sortb};
      while (p_insertb->get_next() != nullptr && *p_insertb->get_next() < *p_temb)
      {
        p_insertb = p_insertb->get_next();
      }
      p_temb->set_next(p_insertb->get_next());
      p_insertb->set_next(p_temb);
      p_temb=nullptr;
      p_insertb=nullptr;
    }
  }
  p_head = p_sortb;
}

void History::update_acb_cgl()
{
  Transaction *p_tem{p_head};
  double acb{};
  double acb_per_share{};
  double cgl{};
  int share_balance{};
  while (p_tem != nullptr)
  {

    if (p_tem->get_trans_type())
    {
      acb = acb + p_tem->get_amount();
      share_balance = share_balance + p_tem->get_shares();

      p_tem->set_cgl(0);
      p_tem->set_acb(acb);
      p_tem->set_share_balance(share_balance);
    }
    else
    {
      acb = acb - (p_tem->get_shares() * acb_per_share);
      share_balance = share_balance - p_tem->get_shares();
      cgl = p_tem->get_amount() - (p_tem->get_shares() * acb_per_share);
      p_tem->set_cgl(cgl);
      p_tem->set_acb(acb);
      p_tem->set_share_balance(share_balance);
    }
    acb_per_share = p_tem->get_acb() / p_tem->get_share_balance();
    p_tem->set_acb_per_share(acb_per_share);
    p_tem = p_tem->get_next();
  }
}

// TASK: compute_cgl() Compute the ACB, and CGL.
double History::compute_cgl(unsigned int year)
{
  double total_cgl{};
  Transaction *p_tem{p_head};
  while (p_tem != nullptr)
  {
    if (p_tem->get_year() == year)
    {
      total_cgl = total_cgl + p_tem->get_cgl();
    }
    p_tem = p_tem->get_next();
  }
  return total_cgl;
}

void History::print()
{
  Transaction *p_tem{p_head};
  std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;
  while (p_tem != nullptr)
  {
    p_tem->print();
    p_tem = p_tem->get_next();
  }
  std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;
};
Transaction *History::get_p_head() { return p_head; }

// Constructor
// TASK 3
//

// Destructor
// TASK 3
//

// read_history(...): Read the transaction history from file.
// TASK 4
//

// insert(...): Insert transaction into linked list.
// TASK 5
//

// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//

// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//

// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8

// print() Print the transaction history.
//TASK 9
//

// GIVEN
// get_p_head(): Full access to the linked list.
//
