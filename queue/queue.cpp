#include <boost/thread.hpp>
#include <iostream>
#include <memory>

namespace kevin_queue {

template<typename ValueT>
class queue {
	std::size_t capacity_{};
	std::size_t size_{};
	std::size_t head_{};
	std::size_t tail_{};
	std::unique_ptr<ValueT[]> data;
    mutable boost::mutex m_;
	
public:
	queue() = default;
	explicit queue(size_t capacity) : capacity_{ capacity }, data{ std::make_unique<ValueT[]>(capacity) } {}

	[[nodiscard]] bool empty() const noexcept { 
        boost::lock_guard<boost::mutex> lg(m_);
        return size_ == 0; 
    }
	[[nodiscard]] std::size_t size() const noexcept { 
        boost::lock_guard<boost::mutex> lg(m_);
        return size_; 
    }
	[[nodiscard]] std::size_t capacity() const noexcept { 
        boost::lock_guard<boost::mutex> lg(m_);
        return capacity_; 
    }
	
	[[nodiscard]] ValueT& front() & {
        boost::lock_guard<boost::mutex> lg(m_);
#ifdef DEBUG
		if (size_ == 0) throw std::runtime_error("queue is empty");
#endif
		return data[tail_];
	}

	[[nodiscard]] const ValueT& front() const & {
        boost::lock_guard<boost::mutex> lg(m_);
#ifdef DEBUG
		if (size_ == 0) throw std::runtime_error("queue is empty");
#endif
		return data[tail_];
	}

	ValueT front() && = delete;

	void pop() {
        boost::lock_guard<boost::mutex> lg(m_);
#ifdef DEBUG
		if (size_ == 0) throw std::runtime_error("queue is empty");
#endif
		if (++tail_ == capacity_) tail_ = 0;
		--size_;
	}
	
	void push(ValueT value) {
        boost::lock_guard<boost::mutex> lg(m_);
		if (size_ == capacity_) resize();
		data[head_] = value;
        if (++head_ == capacity_) head_ = 0;
		++size_;
	}

private:
	void resize() {
        boost::lock_guard<boost::mutex> lg(m_);
		if (capacity_ == 0) capacity_ = 1;
		else capacity_ *= 2;
		std::unique_ptr<ValueT[]> tmp = std::make_unique<ValueT[]>(capacity_);
		for (int i = 0; i < size_; ++i)
			tmp[i] = data[(tail_ + i) % size_];

		data = std::move(tmp);
		tail_ = 0;
		head_ = size_;
	}

};
}

