#include <iostream>
// Forward declaration
template <typename ItemType, int MAX_SIZE>
class RingQueue;

template <typename ItemType, int MAX_SIZE>
class RingQueue{
    public: 
        class iterator;
    typedef ItemType* pointer;
    typedef ItemType& reference;
    public:
        class iterator{
            private:
                RingQueue* parent;
                int offset;

            private:  
                iterator(RingQueue* _parent, int _offset = 0 )
                  : parent(_parent), offset(_offset) { }
            friend class RingQueue<ItemType,MAX_SIZE>;

            public:
                reference operator*() {
                    return parent->buffer[offset] ;  
                }

                iterator& operator++(){
                    this->offset=(this->offset+1)%MAX_SIZE;
                    return *this;
                }

                iterator operator++( int unused ){
                    return *this;
                }

                bool operator==( const iterator& rhs ) const {
                    if(this->parent==rhs.parent && this->offset==rhs.offset)
                        return true;
                    else
                        return false;
                }

                bool operator!=( const iterator& rhs ) const {
                    if (*this==rhs)
                        return false;
                    else
                        return true;
                }
        };
        /**
        class const_iterator{
            private:
                RingQueue* parent;
                int offset;

            private:
                // Only RingQueue objects can create const_iterators...
                const_iterator() ;

            public:
                // ... however, const_iterators can be 'copied'.
                const_iterator( const const_iterator& ) ;

            friend class RingQueue<ItemType,MAX_SIZE>;
        };
        */
    // Friendship goes both ways here.
    friend class iterator;
    // friend class const_iterator;  // not implemented... yet.
    private:
        ItemType buffer[MAX_SIZE];
        int begin_index;
        int ring_size;
        int end_index() const {
            int end= (begin_index+ring_size)%MAX_SIZE;
            return end;
        }
    public: 
        RingQueue() : begin_index(0), ring_size(0) { }

        ItemType front() const { 
            if ( ring_size == 0 ) std::cerr<< "Warning: Empty ring!\n" ;
            return buffer[begin_index];
        }
        ItemType back() const {  
            if ( ring_size == 0 ) std::cerr<< "Warning: Empty ring!\n" ;
            return buffer[end_index()-1];
        }

        void push_back( const ItemType& value ){
            buffer[end_index()]=value;
            if(begin_index==end_index() && ring_size==MAX_SIZE)
                begin_index=(begin_index+1)%MAX_SIZE;   //We add %MAX_SIZE to loop it back to 0 if it crosses buffer[ring_size()-1]
            if(ring_size<MAX_SIZE){
                ring_size++;
            }
            
            return;
        }
        void pop_front(){
            //Add special case when the ring is only one element (or 0 elements) long. 
            begin_index=(begin_index+1)%MAX_SIZE;
            ring_size--;
            return;
        }
        iterator begin() { 
            return iterator(this,begin_index); 
        }
        iterator end() {
            return iterator(this,end_index());
        }
        size_t size() const {
            return ring_size;
        }
        void dump_queue() const {
            std::cout << "Raw queue...\n";
            for ( size_t i = 0 ; i < MAX_SIZE ; ++i )
                std::cout << "Val: " << buffer[i] << ", at: " << buffer+i << '\n';
            std::cout << '\n';
            return;
        }
        int return_begin() const{
            return begin_index;
        }
        int return_end() const{
            return end_index();
        }
};

int main(){
    RingQueue<int,7> rq;
    rq.dump_queue();

    for ( int i = 0 ; i < 7 ; ++i )
        rq.push_back(i+1);
    rq.push_back(8);
    rq.dump_queue();
    rq.pop_front();
    std::cout << "Queue via size: \n";

    // RingQueue<int,7>::iterator it = rq.begin() ; 
    auto it = rq.begin() ; 
    for ( size_t i = 0 ; i < rq.size() ; ++i ) {
        std::cout << "Value: " << *it << ", address: " << &(*it) << '\n';
        ++it;
    }
    std::cout << '\n';
    std::cout << "Queue via iterators: \n";
    for ( auto it = rq.begin() ; it != rq.end() ; ++it ) {
        std::cout << "Value: " << *it << ", address: " << &(*it) << '\n';
    }
    std::cout << '\n';
    rq.dump_queue();
    return 0;
}



/** 
    +++++++++++++++++++++++++++++++++++++++++++++++
    The output of your program [once the missing 
    code is added] should look more or less like
    the output below.
    
    Note:
        It is dependent on the device where 
        this driver is executed. However, it
        should be clear that the difference 
        between consecutive memory addresses
        is equal to the number reported by
        'size_of( int )'.
    +++++++++++++++++++++++++++++++++++++++++++++++

        Raw queue...
        Val: 2, at: 0x7ffcdeeaab40
        Val: 0, at: 0x7ffcdeeaab44
        Val: 4198285, at: 0x7ffcdeeaab48
        Val: 0, at: 0x7ffcdeeaab4c
        Val: 0, at: 0x7ffcdeeaab50
        Val: 0, at: 0x7ffcdeeaab54
        Val: 0, at: 0x7ffcdeeaab58

        Raw queue...
        Val: 8, at: 0x7ffcdeeaab40
        Val: 2, at: 0x7ffcdeeaab44
        Val: 3, at: 0x7ffcdeeaab48
        Val: 4, at: 0x7ffcdeeaab4c
        Val: 5, at: 0x7ffcdeeaab50
        Val: 6, at: 0x7ffcdeeaab54
        Val: 7, at: 0x7ffcdeeaab58

        Queue via size: 
        Value: 3, address: 0x7ffcdeeaab48
        Value: 4, address: 0x7ffcdeeaab4c
        Value: 5, address: 0x7ffcdeeaab50
        Value: 6, address: 0x7ffcdeeaab54
        Value: 7, address: 0x7ffcdeeaab58
        Value: 8, address: 0x7ffcdeeaab40

        Queue via iterators: 
        Value: 3, address: 0x7ffcdeeaab48
        Value: 4, address: 0x7ffcdeeaab4c
        Value: 5, address: 0x7ffcdeeaab50
        Value: 6, address: 0x7ffcdeeaab54
        Value: 7, address: 0x7ffcdeeaab58
        Value: 8, address: 0x7ffcdeeaab40

        Raw queue...
        Val: 8, at: 0x7ffcdeeaab40
        Val: 2, at: 0x7ffcdeeaab44
        Val: 3, at: 0x7ffcdeeaab48
        Val: 4, at: 0x7ffcdeeaab4c
        Val: 5, at: 0x7ffcdeeaab50
        Val: 6, at: 0x7ffcdeeaab54
        Val: 7, at: 0x7ffcdeeaab58
 */