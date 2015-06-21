#include <deque>
#include <mutex>
#include <condition_variable>
#include <memory>

class OrderedLock
{
  mutable std::mutex m_lock;
  std::deque<CvPtr> m_cvs;
  std::condition_variable m_emptyCvs;
public:
  void lock ()
  {
    std::unique_lock<std::mutex> lk (m_lock);
    CvPtr cvptr (new std::condition_variable);
    m_cvs.push_back(cvptr);
    m_emptyCvs.notify_one();
    cvptr->wait(lk);
  }

  void unlock ()
  {
    std::unique_lock<std::mutex> lk (m_lock);
    while (!m_cvs.size())
      m_emptyCvs.wait(lk);

    CvPtr cv = m_cvs.front();
    m_cvs.pop_front();
    cv->notify_one();
  }
};
