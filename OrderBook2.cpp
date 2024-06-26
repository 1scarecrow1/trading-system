#include <bits/stdc++.h>

using namespace std;


enum ordertype {LIMIT, MARKET};

enum side {BUY, SELL};


class Order{

public:
    Order(long timestamp_, side orderside_, unsigned int id_, unsigned int price_, unsigned int quantity_,
          const char *venue_, const char *symbol_, ordertype type_): timestamp(timestamp_), orderside(orderside_), id(id_), price(price_), quantity(quantity_), 
          type(type_){
              strncpy(venue, venue_, sizeof(venue)-1);
              venue[sizeof(venue)-1] = '\0';
              strncpy(symbol, symbol_, sizeof(symbol)-1);
              symbol[sizeof(symbol)-1] ='\0';              
          }

    ~Order(){}

    const char *getVenue(){
        return venue;
    }

    const char *getSymbol(){
        return symbol;
    }

    unsigned int getID(){
        return id;
    }

    ordertype getOrderType(){
        return type;
    }

    unsigned int getQuantity(){
        return quantity;
    }

    unsigned int getPrice(){
        return price;
    }
    
    side getSide(){
        return orderside;
    }

    void setVenue(const char *venue_){
        strncpy(venue, venue_, sizeof(venue) - 1); 
        venue[sizeof(venue) - 1] = '\0'; 
    }

    void setQuantity(unsigned int quantity_){
        quantity = quantity_;
    }

    bool is_valid(){
        return price >= 0 && quantity >= 0 && venue[0]!='\0';
    };


private:
    unsigned int id;
    long timestamp;
    char venue[20];
    char symbol[20];
    unsigned int price;
    unsigned int quantity;
    enum side orderside;
    enum ordertype type;   

};

class OrderBook{

private:
    Order *order_bids[20]{};
    Order *order_offers[20]{};
    int number_of_bids=0;
    int number_of_offers=0;

public:
    OrderBook(){}

    ~OrderBook(){
        clearBooks();
    }

    bool add_order(long timestamp_, side orderside_, unsigned int id_, unsigned int price_,
                   unsigned int quantity_, const char* venue_, const char* symbol_,  ordertype type_){
        unsigned int index = id_;
        Order **orders = (orderside_ == BUY) ? order_bids : order_offers;

        if (orders[index] != NULL) {
            return false; 
        }

        orders[index] = new Order(timestamp_, orderside_, id_, price_, quantity_, venue_, symbol_, type_);
        if (orderside_ == BUY) {
            number_of_bids++;
        } else {
            number_of_offers++;
        }
        return true;
    }

    bool modify_order(side side_, unsigned int id_, unsigned int quantity_){
        Order **orders = (side_==BUY) ? order_bids : order_offers;
        unsigned int index = id_;
        if (orders[index]->getSide()==side_ && orders[index]->getID()==id_) {
            orders[index]->setQuantity(quantity_);
            return true;
        }
        else return false;
    }

    bool remove_order(side side_, unsigned int id_){
        Order **orders = (side_==BUY) ? order_bids : order_offers;
        if (orders[id_] != NULL){
            delete orders[id_];
            return true;
        }
        else return false;
    }

    void clearBooks() {
        for (int k = 0; k < 20; k++) {
            if (order_bids[k] != NULL) {
                delete order_bids[k];
                order_bids[k] = NULL;
                number_of_bids--;
            }
            if (order_offers[k] != NULL) {
                delete order_offers[k];
                order_offers[k] = NULL;
                number_of_offers--;
            }
        }
    }

    Order* getOrderBids(const int a)
    {
        return order_bids[a];
    }

    Order* getOrderOffers(const int a)
    {
        return order_offers[a];
    }

    int getNumberOfBids()
    {
        return number_of_bids;
    }

    int getNumberOfOffers()

    {

        return number_of_offers;

    }
};
int test_count = 0;
int pass_count = 0;

#define TEST_FUNCTION(X,Y) { \
    test_count++; \
    if (X==Y) { \
        std::cout << "Line " << __LINE__ << ": PASSED" << std::endl; \
        pass_count++; \
    } else { \
        std::cout << "Line " << __LINE__ << ": FAILED - Expected '"; \
        std::cout << Y << "' Recieved '" << X << "'" << std::endl; \
    } \
}

#define TEST_STRING(X,Y) { \
    test_count++; \
    if (strcmp(X,Y)==0) { \
        std::cout << "Line " << __LINE__ << ": PASSED" << std::endl; \
        pass_count++; \
    } else { \
        std::cout << "Line " << __LINE__ << ": FAILED - Expected '"; \
        std::cout << Y << "' Recieved '" << X << "'" << std::endl; \
    } \
}

#define PRINT_RESULTS() { \
    std::cout << std::endl; \
    std::cout << "RESULTS: "; \
    std::cout << "You passed " << pass_count << " out of "; \
    std::cout << test_count << " test cases." << std::endl; \
}

int main()
{
    Order o1(100,side::BUY,1,10,1000,"JPM","APPL",ordertype::LIMIT);

    TEST_FUNCTION(o1.getID(),1);
    TEST_FUNCTION(o1.getOrderType(),ordertype::LIMIT);
    TEST_FUNCTION(o1.getPrice(),10);
    TEST_FUNCTION(o1.getQuantity(),1000);
    TEST_STRING(o1.getVenue(),"JPM");
    TEST_STRING(o1.getSymbol(),"APPL");
    TEST_FUNCTION(o1.is_valid(), true);
    o1.setVenue("\0");
    TEST_FUNCTION(o1.is_valid(), false);
    o1.setVenue("BARX");
    TEST_FUNCTION(o1.is_valid(), true);

    // Create an order book on the stack

    OrderBook b1;

    // Add an order to the book

    TEST_FUNCTION(b1.add_order(100, side::BUY,1,10,1000,"JPM","APPL",ordertype::LIMIT),true);
    TEST_FUNCTION(b1.add_order(100,side::BUY,2,11,1000,"BARX","APPL",ordertype::MARKET),true);

    TEST_STRING(b1.getOrderBids(1)->getVenue(), "JPM");
    TEST_STRING(b1.getOrderBids(2)->getVenue(), "BARX");
    TEST_FUNCTION(b1.getOrderBids(2)->getOrderType(), ordertype::MARKET);

    TEST_FUNCTION(b1.add_order(100,side::SELL,4,11,1000,"BARX","APPL",ordertype::MARKET),true);
    TEST_FUNCTION(b1.getOrderOffers(4)->getOrderType(), ordertype::MARKET);
    TEST_FUNCTION(b1.add_order(100,side::SELL,4,11,1000,"BARX","APPL",ordertype::MARKET),false);

    TEST_FUNCTION(b1.getNumberOfBids(),2);
    TEST_FUNCTION(b1.getNumberOfOffers(),1);

    //Reset the book
    b1.clearBooks();

    TEST_FUNCTION(b1.getNumberOfBids(),0);
    TEST_FUNCTION(b1.getNumberOfOffers(),0);

    int count_offer_null=0;
    int count_bid_null=0;
    for(int k=0;k<20;k++)
    {
        count_offer_null+=(b1.getOrderOffers(k)==NULL)?1:0;
        count_bid_null+=(b1.getOrderBids(k)==NULL)?1:0;
    }
    TEST_FUNCTION(count_offer_null+count_bid_null,40);

    for(int k=0;k<20;k++)
    {
        b1.add_order(100,side::SELL,k,50+k,1000-k*10,"BARX","APPL",ordertype::LIMIT);
        b1.add_order(100,side::BUY,k,50-k,1000+k*100,"BARX","APPL",ordertype::LIMIT);
    }

    TEST_FUNCTION(b1.getNumberOfBids(),20);
    TEST_FUNCTION(b1.getNumberOfOffers(),20);

    unsigned int sum_quantity = 0;
    for(int k=0;k<20;k++)
    {
        sum_quantity+=b1.getOrderOffers(k)->getQuantity();
        sum_quantity+=b1.getOrderBids(k)->getQuantity();
    }

    TEST_FUNCTION(sum_quantity,57100);

    for(int k=0;k<20;k++)
    {
        b1.modify_order(side::BUY,k,b1.getOrderBids(k)->getQuantity()-100);
        b1.modify_order(side::SELL,k,b1.getOrderOffers(k)->getQuantity()-100);
    }

    sum_quantity = 0;
    for(int k=0;k<20;k++)
    {
        sum_quantity+=b1.getOrderOffers(k)->getQuantity();
        sum_quantity+=b1.getOrderBids(k)->getQuantity();
    }


    TEST_FUNCTION(sum_quantity,57100-100*40);

    PRINT_RESULTS();
    return 0;
}
