#include "polygon.hpp"

std::istream& chernikova::operator>>(std::istream& in, chernikova::Point& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> chernikova::DelimiterI{'(' };
  in >> dest.x;
  in >> chernikova::DelimiterI{';' };
  in >> dest.y;
  in >> chernikova::DelimiterI{')' };
  return in;
}

std::istream& chernikova::operator>>(std::istream& in, chernikova::Polygon& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t count = 0;
  in >> count;
  if (count < 3)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  using iter = std::istream_iterator< chernikova::Point >;
  dest.points.clear();
  std::copy_n(iter(in), count, std::back_inserter(dest.points));
  if (dest.points.size() != count)
  {
    in.setstate(std::ios::failbit);
    return in;
  }
  return in;
}

bool chernikova::isEven(const chernikova::Polygon& polygon)
{
  return polygon.points.size() % 2 == 0;
}

bool chernikova::isOdd(const chernikova::Polygon& polygon)
{
  return !isEven(polygon);
}

double chernikova::calcArea(const chernikova::Point& left, const chernikova::Point& right)
{
  return 0.5 * (left.x * right.y - right.x * left.y);
}

double chernikova::getArea(const chernikova::Polygon& polygon)
{
  double res = 0.0;
  for (size_t i = 0; i < polygon.points.size() - 1; ++i)
  {
    res += chernikova::calcArea(polygon.points[i], polygon.points[i + 1]);
  }
  res += chernikova::calcArea(polygon.points.back(), polygon.points.front());
  return std::abs(res);
}

double chernikova::sumArea(double cur, const chernikova::Polygon& polygon)
{
  return cur + chernikova::getArea(polygon);
}

bool chernikova::isNecessaryVertex(const chernikova::Polygon& polygon, size_t count)
{
  return polygon.points.size() == count;
}

double chernikova::chooseGreaterArea(double cur, const Polygon& polygon)
{
  double area = chernikova::getArea(polygon);
  return (cur > area) ? cur : area;
}

size_t chernikova::chooseGreaterVertexes(double cur, const Polygon& polygon)
{
  size_t count = polygon.points.size();
  return (cur > count) ? cur : count;
}

double chernikova::chooseLessArea(double cur, const chernikova::Polygon& polygon)
{
  double area = chernikova::getArea(polygon);
  return (cur < area) ? cur : area;
}

size_t chernikova::chooseLessVertexes(double cur, const chernikova::Polygon& polygon)
{
  size_t count = polygon.points.size();
  return (cur < count) ? cur : count;
}

void chernikova::getAreaEven(const std::vector< Polygon >& polygons, std::ostream& out)
{
  std::vector< Polygon > even_polygons;
  std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(even_polygons), chernikova::isEven);
  StreamGuard streamGuard(out);
  out << std::fixed << std::setprecision(1);
  out << std::accumulate(even_polygons.begin(), even_polygons.end(), 0.0, chernikova::sumArea) << "\n";
}

void chernikova::getAreaOdd(const std::vector< Polygon >& polygons, std::ostream& out)
{
  std::vector< Polygon > odd_polygons;
  std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(odd_polygons), chernikova::isOdd);
  StreamGuard streamGuard(out);
  out << std::fixed << std::setprecision(1);
  out << std::accumulate(odd_polygons.begin(), odd_polygons.end(), 0.0, chernikova::sumArea) << "\n";
}

void chernikova::getAreaMean(const std::vector< Polygon >& polygons, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  size_t count = polygons.size();
  StreamGuard streamGuard(out);
  out << std::fixed << std::setprecision(1);
  out << std::accumulate(polygons.begin(), polygons.end(), 0.0, chernikova::sumArea) / count << "\n";
}

void chernikova::getAreaVertexes(const std::vector< Polygon >& polygons, size_t count, std::ostream& out)
{
  if (count < 3)
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  std::vector< Polygon > vertexes_polygons;
  using namespace std::placeholders;
  auto pred = std::bind(chernikova::isNecessaryVertex, _1, count);
  std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(vertexes_polygons), pred);
  StreamGuard streamGuard(out);
  out << std::fixed << std::setprecision(1);
  out << std::accumulate(vertexes_polygons.begin(), vertexes_polygons.end(), 0.0, sumArea) << "\n";
}

void chernikova::getMaxArea(const std::vector< Polygon >& polygons, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  StreamGuard streamGuard(out);
  out << std::fixed << std::setprecision(1);
  out << std::accumulate(polygons.begin(), polygons.end(), 0.0, chernikova::chooseGreaterArea) << "\n";
}

void chernikova::getMaxVertexes(const std::vector< Polygon >& polygons, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  StreamGuard streamGuard(out);
  out << std::accumulate(polygons.begin(), polygons.end(), 0, chernikova::chooseGreaterVertexes) << "\n";
}

void chernikova::getMinArea(const std::vector< Polygon >& polygons, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  StreamGuard streamGuard(out);
  out << std::fixed << std::setprecision(1);
  out << std::accumulate(polygons.begin(), polygons.end(), chernikova::getArea(polygons.front()), chernikova::chooseLessArea) << "\n";
}

void chernikova::getMinVertexes(const std::vector< Polygon >& polygons, std::ostream& out)
{
  if (polygons.empty())
  {
    throw std::logic_error("<INVALID COMMAND>");
  }
  StreamGuard streamGuard(out);
  out << std::accumulate(polygons.begin(), polygons.end(), polygons.front().points.size(), chernikova::chooseLessVertexes) << "\n";
}
