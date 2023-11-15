//
// Created by ankush on 4/17/23.
//


#include <pybind11/pybind11.h>
#include "PosixMessageQueueWrapper.h"

namespace py = pybind11;

PYBIND11_MODULE(posix_message_queue_wrapper, m) {
      py::class_<PosixMessageQueueWrapper>(m, "PosixMessageQueueWrapper")
          .def(py::init<const std::string &, const std::string &, const std::string &>())
          .def("sendMessage", &PosixMessageQueueWrapper::sendMessage)
          .def("recvMessage", &PosixMessageQueueWrapper::recvMessage)
          .def("registerPublisher", &PosixMessageQueueWrapper::registerPublisher)
          .def("registerSubscriber", &PosixMessageQueueWrapper::registerSubscriber);
}
