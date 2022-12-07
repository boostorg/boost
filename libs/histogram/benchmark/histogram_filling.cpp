// Copyright 2015-2019 Hans Dembinski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <benchmark/benchmark.h>
#include <boost/histogram/axis/regular.hpp>
#include <boost/histogram/storage_adaptor.hpp>
#include <memory>
#include "../test/throw_exception.hpp"
#include "../test/utility_histogram.hpp"
#include "generator.hpp"

#include <cassert>
struct assert_check {
  assert_check() {
    assert(false); // don't run with asserts enabled
  }
} _;

using SStore = std::vector<double>;

// make benchmark compatible with older versions of the library
#if __has_include(<boost/histogram/unlimited_storage.hpp>)
#include <boost/histogram/unlimited_storage.hpp>
using DStore = boost::histogram::unlimited_storage<>;
#else
#include <boost/histogram/adaptive_storage.hpp>
using DStore = boost::histogram::adaptive_storage<>;
#endif

using namespace boost::histogram;
namespace op = boost::histogram::axis::option;
using reg = axis::regular<>;
using reg_closed =
    axis::regular<double, boost::use_default, boost::use_default, op::none_t>;

class reg_closed_unsafe {
public:
  reg_closed_unsafe(axis::index_type n, double start, double stop)
      : min_{start}, delta_{stop - start}, size_{n} {}

  axis::index_type index(double x) const noexcept {
    // Runs in hot loop, please measure impact of changes
    auto z = (x - min_) / delta_;
    // assume that z < 0 and z > 1 never happens, promised by inclusive()
    if (z == 1) return size() - 1;
    return static_cast<axis::index_type>(z * size());
  }

  axis::index_type size() const noexcept { return size_; }

  static constexpr bool inclusive() { return true; }

private:
  double min_;
  double delta_;
  axis::index_type size_;
};

template <class Distribution, class Tag, class Storage = SStore>
static void fill_1d(benchmark::State& state) {
  auto h = make_s(Tag(), Storage(), reg(100, 0, 1));
  auto gen = generator<Distribution>();
  for (auto _ : state) benchmark::DoNotOptimize(h(gen()));
  state.SetItemsProcessed(state.iterations());
}

template <class Distribution, class Tag, class Storage = SStore>
static void fill_1d_closed(benchmark::State& state) {
  auto h = make_s(Tag(), Storage(), reg_closed(100, 0, 1));
  auto gen = generator<Distribution>();
  for (auto _ : state) benchmark::DoNotOptimize(h(gen()));
  state.SetItemsProcessed(state.iterations());
}

template <class Distribution, class Tag, class Storage = SStore>
static void fill_1d_closed_unsafe(benchmark::State& state) {
  auto h = make_s(Tag(), Storage(), reg_closed_unsafe(100, 0, 1));
  auto gen = generator<Distribution>();
  for (auto _ : state) benchmark::DoNotOptimize(h(gen()));
  state.SetItemsProcessed(state.iterations());
}

template <class Distribution, class Tag, class Storage = SStore>
static void fill_n_1d(benchmark::State& state) {
  auto h = make_s(Tag(), Storage(), reg(100, 0, 1));
  auto gen = generator<Distribution>();
  for (auto _ : state) h.fill(gen);
  state.SetItemsProcessed(state.iterations() * gen.size());
}

template <class Distribution, class Tag, class Storage = SStore>
static void fill_n_1d_closed(benchmark::State& state) {
  auto h = make_s(Tag(), Storage(), reg_closed(100, 0, 1));
  auto gen = generator<Distribution>();
  for (auto _ : state) h.fill(gen);
  state.SetItemsProcessed(state.iterations() * gen.size());
}

template <class Distribution, class Tag, class Storage = SStore>
static void fill_n_1d_closed_unsafe(benchmark::State& state) {
  auto h = make_s(Tag(), Storage(), reg_closed_unsafe(100, 0, 1));
  auto gen = generator<Distribution>();
  for (auto _ : state) h.fill(gen);
  state.SetItemsProcessed(state.iterations() * gen.size());
}

template <class Distribution, class Tag, class Storage = SStore>
static void fill_2d(benchmark::State& state) {
  auto h = make_s(Tag(), Storage(), reg(100, 0, 1), reg(100, 0, 1));
  auto gen = generator<Distribution>();
  for (auto _ : state) benchmark::DoNotOptimize(h(gen(), gen()));
  state.SetItemsProcessed(state.iterations() * 2);
}

template <class Distribution, class Tag, class Storage = SStore>
static void fill_n_2d(benchmark::State& state) {
  auto h = make_s(Tag(), Storage(), reg(100, 0, 1), reg(100, 0, 1));
  auto gen = generator<Distribution>();
  auto v = {gen, gen};
  for (auto _ : state) h.fill(v);
  state.SetItemsProcessed(state.iterations() * 2 * gen.size());
}

template <class Distribution, class Tag, class Storage = SStore>
static void fill_3d(benchmark::State& state) {
  auto h = make_s(Tag(), Storage(), reg(100, 0, 1), reg(100, 0, 1), reg(100, 0, 1));
  auto gen = generator<Distribution>();
  for (auto _ : state) benchmark::DoNotOptimize(h(gen(), gen(), gen()));
  state.SetItemsProcessed(state.iterations() * 3);
}

template <class Distribution, class Tag, class Storage = SStore>
static void fill_n_3d(benchmark::State& state) {
  auto h = make_s(Tag(), Storage(), reg(100, 0, 1), reg(100, 0, 1), reg(100, 0, 1));
  auto gen = generator<Distribution>();
  auto v = {gen, gen, gen};
  for (auto _ : state) h.fill(v);
  state.SetItemsProcessed(state.iterations() * 3 * gen.size());
}

template <class Distribution, class Tag, class Storage = SStore>
static void fill_6d(benchmark::State& state) {
  auto h = make_s(Tag(), Storage(), reg(10, 0, 1), reg(10, 0, 1), reg(10, 0, 1),
                  reg(10, 0, 1), reg(10, 0, 1), reg(10, 0, 1));
  auto gen = generator<Distribution>();
  for (auto _ : state)
    benchmark::DoNotOptimize(h(gen(), gen(), gen(), gen(), gen(), gen()));
  state.SetItemsProcessed(state.iterations() * 6);
}

template <class Distribution, class Tag, class Storage = SStore>
static void fill_n_6d(benchmark::State& state) {
  auto h = make_s(Tag(), Storage(), reg(10, 0, 1), reg(10, 0, 1), reg(10, 0, 1),
                  reg(10, 0, 1), reg(10, 0, 1), reg(10, 0, 1));
  auto gen = generator<Distribution>();
  auto v = {gen, gen, gen, gen, gen, gen};
  for (auto _ : state) h.fill(v);
  state.SetItemsProcessed(state.iterations() * 6 * gen.size());
}

BENCHMARK_TEMPLATE(fill_1d, uniform, static_tag);
// BENCHMARK_TEMPLATE(fill_1d, uniform, static_tag, DStore);
BENCHMARK_TEMPLATE(fill_1d, normal, static_tag);
// BENCHMARK_TEMPLATE(fill_1d, normal, static_tag, DStore);
BENCHMARK_TEMPLATE(fill_1d, uniform, dynamic_tag);
// BENCHMARK_TEMPLATE(fill_1d, uniform, dynamic_tag, DStore);
BENCHMARK_TEMPLATE(fill_1d, normal, dynamic_tag);
// BENCHMARK_TEMPLATE(fill_1d, normal, dynamic_tag, DStore);
BENCHMARK_TEMPLATE(fill_1d_closed, uniform, static_tag);
BENCHMARK_TEMPLATE(fill_1d_closed_unsafe, uniform, static_tag);

BENCHMARK_TEMPLATE(fill_n_1d, uniform, static_tag);
// BENCHMARK_TEMPLATE(fill_n_1d, uniform, static_tag, DStore);
BENCHMARK_TEMPLATE(fill_n_1d, normal, static_tag);
// BENCHMARK_TEMPLATE(fill_n_1d, normal, static_tag, DStore);
BENCHMARK_TEMPLATE(fill_n_1d, uniform, dynamic_tag);
// BENCHMARK_TEMPLATE(fill_n_1d, uniform, dynamic_tag, DStore);
BENCHMARK_TEMPLATE(fill_n_1d, normal, dynamic_tag);
// BENCHMARK_TEMPLATE(fill_n_1d, normal, dynamic_tag, DStore);
BENCHMARK_TEMPLATE(fill_n_1d_closed, uniform, static_tag);
BENCHMARK_TEMPLATE(fill_n_1d_closed_unsafe, uniform, static_tag);

BENCHMARK_TEMPLATE(fill_2d, uniform, static_tag);
// BENCHMARK_TEMPLATE(fill_2d, uniform, static_tag, DStore);
BENCHMARK_TEMPLATE(fill_2d, normal, static_tag);
// BENCHMARK_TEMPLATE(fill_2d, normal, static_tag, DStore);
BENCHMARK_TEMPLATE(fill_2d, uniform, dynamic_tag);
// BENCHMARK_TEMPLATE(fill_2d, uniform, dynamic_tag, DStore);
BENCHMARK_TEMPLATE(fill_2d, normal, dynamic_tag);
// BENCHMARK_TEMPLATE(fill_2d, normal, dynamic_tag, DStore);

BENCHMARK_TEMPLATE(fill_n_2d, uniform, static_tag);
// BENCHMARK_TEMPLATE(fill_n_2d, uniform, static_tag, DStore);
BENCHMARK_TEMPLATE(fill_n_2d, normal, static_tag);
// BENCHMARK_TEMPLATE(fill_n_2d, normal, static_tag, DStore);
BENCHMARK_TEMPLATE(fill_n_2d, uniform, dynamic_tag);
// BENCHMARK_TEMPLATE(fill_n_2d, uniform, dynamic_tag, DStore);
BENCHMARK_TEMPLATE(fill_n_2d, normal, dynamic_tag);
// BENCHMARK_TEMPLATE(fill_n_2d, normal, dynamic_tag, DStore);

BENCHMARK_TEMPLATE(fill_3d, uniform, static_tag);
// BENCHMARK_TEMPLATE(fill_3d, uniform, static_tag, DStore);
BENCHMARK_TEMPLATE(fill_3d, normal, static_tag);
// BENCHMARK_TEMPLATE(fill_3d, normal, static_tag, DStore);
BENCHMARK_TEMPLATE(fill_3d, uniform, dynamic_tag);
// BENCHMARK_TEMPLATE(fill_3d, uniform, dynamic_tag, DStore);
BENCHMARK_TEMPLATE(fill_3d, normal, dynamic_tag);
// BENCHMARK_TEMPLATE(fill_3d, normal, dynamic_tag, DStore);

BENCHMARK_TEMPLATE(fill_n_3d, uniform, static_tag);
// BENCHMARK_TEMPLATE(fill_n_3d, uniform, static_tag, DStore);
BENCHMARK_TEMPLATE(fill_n_3d, normal, static_tag);
// BENCHMARK_TEMPLATE(fill_n_3d, normal, static_tag, DStore);
BENCHMARK_TEMPLATE(fill_n_3d, uniform, dynamic_tag);
// BENCHMARK_TEMPLATE(fill_n_3d, uniform, dynamic_tag, DStore);
BENCHMARK_TEMPLATE(fill_n_3d, normal, dynamic_tag);
// BENCHMARK_TEMPLATE(fill_n_3d, normal, dynamic_tag, DStore);

BENCHMARK_TEMPLATE(fill_6d, uniform, static_tag);
// BENCHMARK_TEMPLATE(fill_6d, uniform, static_tag, DStore);
BENCHMARK_TEMPLATE(fill_6d, normal, static_tag);
// BENCHMARK_TEMPLATE(fill_6d, normal, static_tag, DStore);
BENCHMARK_TEMPLATE(fill_6d, uniform, dynamic_tag);
// BENCHMARK_TEMPLATE(fill_6d, uniform, dynamic_tag, DStore);
BENCHMARK_TEMPLATE(fill_6d, normal, dynamic_tag);
// BENCHMARK_TEMPLATE(fill_6d, normal, dynamic_tag, DStore);

BENCHMARK_TEMPLATE(fill_n_6d, uniform, static_tag);
// BENCHMARK_TEMPLATE(fill_n_6d, uniform, static_tag, DStore);
BENCHMARK_TEMPLATE(fill_n_6d, normal, static_tag);
// BENCHMARK_TEMPLATE(fill_n_6d, normal, static_tag, DStore);
BENCHMARK_TEMPLATE(fill_n_6d, uniform, dynamic_tag);
// BENCHMARK_TEMPLATE(fill_n_6d, uniform, dynamic_tag, DStore);
BENCHMARK_TEMPLATE(fill_n_6d, normal, dynamic_tag);
// BENCHMARK_TEMPLATE(fill_n_6d, normal, dynamic_tag, DStore);
