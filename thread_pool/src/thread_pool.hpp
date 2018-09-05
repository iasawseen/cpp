#ifndef __THREAD_POOL__
#define __THREAD_POOL__

#include <atomic>
#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <future>
#include <iostream>
#include <typeinfo>
#include <algorithm>

namespace au
{

	class thread_pool {

        class thread_wrapper;
        class joiner;

        using thread_t = std::unique_ptr<thread_wrapper>;
        using flag_t = std::shared_ptr<bool>;
        using task_type = std::function<void()>;

        class thread_wrapper {
            flag_t flag_ = std::make_shared<bool>(true);
            std::thread thr_;

        public:
            template<class func_t, class... Args>
            explicit thread_wrapper(func_t&& function, Args&&... args)
                    : thr_(std::thread(std::forward<func_t>(function),
                                       std::forward<Args>(args)..., flag_))
            {}

            thread_wrapper() = delete;
            thread_wrapper(thread_wrapper&&) noexcept = default;
            thread_wrapper& operator=(thread_wrapper&&) noexcept = default;
            thread_wrapper(thread_wrapper const&) = delete;
            thread_wrapper& operator=(thread_wrapper const&) = delete;

            ~thread_wrapper() {
                if (thr_.joinable()) {
                    thr_.join();
                }
            }

            bool joinable() const {
                return thr_.joinable();
            }

            void join() {
                thr_.join();
            }

            bool get_flag() const {
                return *flag_;
            }

            void set_flag(bool new_flag) {
                *flag_ = new_flag;
            }
        };

        class join_threads {
            std::vector<thread_t>& threads_;
        public:
            explicit join_threads(std::vector<thread_t>& threads)
                    : threads_(threads) {}

            join_threads() = delete;
            join_threads(join_threads const&) = delete;
            join_threads(join_threads&&) = delete;
            join_threads& operator=(join_threads const&) = delete;
            join_threads& operator=(join_threads&&) = delete;

            ~join_threads() {
                for (auto& thread : threads_) {
                    if (thread->joinable()) {
                        thread->join();
                    }
                }
            }
        };

		std::atomic_bool done_;

        std::atomic_size_t threads_count_;
		std::atomic_size_t cur_threads_count_;
		std::atomic_size_t deact_threads_count_;

		std::atomic_size_t max_queue_size_;

        std::mutex queue_mutex_;
        std::mutex threads_mutex_;

        std::condition_variable queue_upd_;
        std::condition_variable threads_upd_;

        std::queue<std::function<void()>> task_queue_;
		std::vector<thread_t> threads_;
        join_threads joins_;

	public:

		thread_pool(size_t threads_count, size_t max_queue_size)
                : done_(false)
                , threads_count_(threads_count)
                , cur_threads_count_(threads_count)
                , deact_threads_count_(0)
                , max_queue_size_(max_queue_size)
                , joins_(threads_)

        {
			for (size_t i = 0; i < threads_count_; ++i) {
                threads_.emplace_back(std::make_unique<thread_wrapper>(&thread_pool::worker_thread, this));
            }
		}

        thread_pool() = delete;
        thread_pool(thread_pool const&) = delete;
        thread_pool(thread_pool&&) = delete;
        thread_pool& operator=(thread_pool const&) = delete;
        thread_pool& operator=(thread_pool&&) = delete;

        ~thread_pool() {
            done_ = true;
        }

        void worker_thread(flag_t const& flag) {

            while(!done_) {
                while(true) {
                    std::unique_lock<std::mutex> queue_locker(queue_mutex_);

                    if (task_queue_.empty()) {
                        std::unique_lock<std::mutex> threads_locker(threads_mutex_);
                        if (cur_threads_count_ > threads_count_) {
                            *flag = false;

                            --cur_threads_count_;
                            ++deact_threads_count_;

                            threads_locker.unlock();

                            threads_upd_.notify_all();
                            return;
                        }

                        std::this_thread::yield();

                        break;
                    }

                    task_type task = std::move(task_queue_.front());
                    task_queue_.pop();

                    queue_locker.unlock();

                    queue_upd_.notify_one();

                    task();
                }
            }
        }

        template<class Func_t, class... Args>
        auto submit(Func_t&& function, Args&&... args) -> std::future<typename std::result_of<Func_t(Args...)>::type> {
            using r_type = typename std::result_of<Func_t(Args...)>::type;

            std::shared_ptr<std::packaged_task<r_type()>> task =
                    std::make_shared<std::packaged_task<r_type()>>(std::bind(
                            std::forward<Func_t>(function),
                            std::forward<Args>(args)...));

            std::future<r_type> result = task->get_future();

            std::unique_lock<std::mutex> locker(queue_mutex_);
            queue_upd_.wait(locker, [this](){ return task_queue_.size() < max_queue_size_;});
            task_queue_.emplace([task](){task->operator()();});

            return result;
        };

		void set_threads_count(size_t threads_count) {
            std::unique_lock<std::mutex> locker(threads_mutex_);

            if (threads_count < 1) {
                return;
            }

            threads_count_ = threads_count;

			if (cur_threads_count_ == threads_count_) {
                return;
            }

            if (cur_threads_count_ < threads_count_) {

                while (cur_threads_count_ < threads_count_) {
                    threads_.emplace_back(std::make_unique<thread_wrapper>(&thread_pool::worker_thread, this));
                    ++cur_threads_count_;
                }

            } else {

                while (cur_threads_count_ > threads_count_) {
                    threads_upd_.wait(locker);

                    while(deact_threads_count_ > 0) {
                        auto thread_iter = std::find_if(threads_.begin(), threads_.end(),
                                                        [](auto& item){ return item->get_flag() == false; });

                        if (thread_iter == threads_.end()) {
                            continue;
                        }

                        threads_.erase(thread_iter);
                        --deact_threads_count_;
                    }

                }
            }
        }

		void set_max_queue_size(size_t max_queue_size) {
            std::unique_lock<std::mutex> locker(queue_mutex_);

            if (max_queue_size < 1) {
                return;
            }
            max_queue_size_ = max_queue_size;

            if (task_queue_.size() <= max_queue_size_) {
                return;
            }

            queue_upd_.wait(locker,
                            [this](){ return task_queue_.size() <= max_queue_size_; });
        }

		size_t threads_count() const {
			return cur_threads_count_;
		}

		size_t max_queue_size() const {
			return max_queue_size_;
		}
	};

} // namespace au

#endif // __THREAD_POOL__
