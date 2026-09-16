#ifndef SHARED_PTR_HEADER
#define SHARED_PTR_HEADER

class ControlBlockBase {
public:
    ControlBlockBase(){
    }; // TODO: implement the default constructor.

    // dtor is virtual, so that we can call derived class's dtor from a ptr to this base class.
    virtual ~ControlBlockBase(){ //had delete here, but I believe that caused a double free
    }; // TODO: implement the destructor.

    // pure virtual function; must be overriden by derived classes
    virtual void* managedAddress() = 0;

    // Delete copies, which also implicitly deletes moves.
    ControlBlockBase(const ControlBlockBase&) = delete;
    ControlBlockBase& operator=(const ControlBlockBase&) = delete;

    long increment()
    {
        // TODO: increment refcount by 1 and return result.
        refcount++;
        return refcount;
    }

    long decrement()
    {
        // TODO: decrement refcount by 1 and return result.
        refcount--;
        return refcount;
    }

    long refCount() const
    {
        // TODO: just return the refcount.
        return refcount;
    }

private:
    // TODO: add field(s) which both control block types need to have
    long refcount = 0;
    
};

template<typename T> class ControlBlock : public ControlBlockBase {
public:
    ControlBlock(T* ptr) : managedPtr(ptr) {
    }

    ~ControlBlock() override {
        delete managedPtr;
    }

    void* managedAddress() override {
        return managedPtr;
    }

private:
    T* managedPtr;
};

template<typename T> class SharedPtr {
    public:
    //Constructors
        SharedPtr() : ptr(nullptr), controlBlock(nullptr) {};
        SharedPtr(T* rawPtr) : ptr(rawPtr), controlBlock(new ControlBlock<T>(rawPtr)) {controlBlock->increment();}; //Add a new count
    //Destructor
        ~SharedPtr() {
            if (controlBlock) { //If control block
                controlBlock->decrement(); //Say I'm leaving
                if (controlBlock->refCount() == 0) { //If no more, delete the control block
                    delete controlBlock;
                }
            }
        }
    //Copy
        SharedPtr(const SharedPtr& newPtr) : ptr(newPtr.ptr), controlBlock(newPtr.controlBlock) {
            if (controlBlock) {
                controlBlock->increment();
            }
        }
    //Copy assignment
        SharedPtr& operator=(const SharedPtr& newPtr) {
            if (this != &newPtr) { //Make sure we're not already copied
                if (controlBlock) { //Get rid of old control block
                    controlBlock->decrement();
                    if (controlBlock->refCount() == 0) {
                        delete controlBlock;
                    }
                }
                ptr = newPtr.ptr; //Change to new pointer
                controlBlock = newPtr.controlBlock;
                if (controlBlock) {
                    controlBlock->increment();
                }
            }
            return *this;
        }
    //Move
        SharedPtr(SharedPtr&& newPtr) : ptr(newPtr.ptr), controlBlock(newPtr.controlBlock) {
            newPtr.ptr = nullptr; //Reset current pointer
            newPtr.controlBlock = nullptr;
        }
    //Move assignment
        SharedPtr& operator=(SharedPtr&& newPtr) {
            if (this != &newPtr) { //Make sure we're not the same
                if (controlBlock) { //Get rid of old control block
                    controlBlock->decrement();
                    if (controlBlock->refCount() == 0) {
                        delete controlBlock;
                    }
                }
                ptr = newPtr.ptr; //Change to new pointer
                controlBlock = newPtr.controlBlock;
                newPtr.ptr = nullptr; //Reset old pointer
                newPtr.controlBlock = nullptr;
            }
            return *this;
        }
    //Dereference operator
        T& operator*() const {
            return *ptr;
        }
    //T* Get
        T* get() {
            return ptr;
        }
    //Comparison operator (and !=?)
        bool operator==(const SharedPtr& other) const {
            return ptr == other.ptr; //Use the raw pointers as they shouldn't be different
        }
    //Boolean conversion
        explicit operator bool() const { //From google, explicit operator bool() const is how you do boolean conversion
            return ptr != nullptr;
        }
    //Void swap (I don't touch the refcounts?)
        void swap(SharedPtr& other) {
            std::swap(ptr, other.ptr);
            std::swap(controlBlock, other.controlBlock);
        }
    //Void reset
        void reset() {
            if (controlBlock) { //Explained this one before, scroll up
                controlBlock->decrement();
                if (controlBlock->refCount() == 0) {
                    delete controlBlock;
                }
            }
            ptr = nullptr;
            controlBlock = nullptr;
        }
        void reset(T* newPtr) {
            if (ptr == newPtr) { //If we're resetting to the same pointer, don't do anything
                return;
            }

            if (controlBlock) {
                controlBlock->decrement();
                if (controlBlock->refCount() == 0) {
                    delete controlBlock;
                }
            }
            ptr = newPtr;
            controlBlock = new ControlBlock<T>(newPtr);
            controlBlock->increment();
        }
    //Usecount
        long useCount() const {
            if (controlBlock) {
                return controlBlock->refCount();
            }
            return 0;
        }
    
    private:
        T* ptr;
        ControlBlockBase* controlBlock;
};

template <typename T, typename... Args> SharedPtr<T> makeSharedBasic(Args&&... args) {
    T* rawPtr = new T(std::forward<Args>(args)...); //Make raw pointer pointing to new T object with forwarded args
    return SharedPtr<T>(rawPtr); //Convert raw pointer to shared pointer
}
#endif
