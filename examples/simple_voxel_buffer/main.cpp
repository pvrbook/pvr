#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

struct VoxelBuffer {
  VoxelBuffer(int sizeX, int sizeY, int sizeZ, float *localToWorld)
    : m_sizeX(sizeX), m_sizeY(sizeY), m_sizeZ(sizeZ), 
      m_localToWorld(localToWorld)
  { m_data.resize(sizeX * sizeY * sizeZ); }
  float* localToWorld() const
  { return m_localToWorld; }
  float read(int i, int j, int k) const
  { return m_data[index(i, j, k)]; }
  void write(int i, int j, int k, float value)
  { m_data[index(i, j, k)] = value; }
  void read(const char *filename)	
  {
    std::ifstream in(filename);
    in >> m_sizeX >> m_sizeY >> m_sizeZ;
    std::copy(std::istream_iterator<float>(in),
              std::istream_iterator<float>(),
              std::back_inserter(m_data));
  }
  void write(const char *filename)
  {
    std::ofstream out(filename);
    out << m_sizeX << " " << m_sizeY << " " << m_sizeZ << std::endl;
    std::copy(m_data.begin(), m_data.end(), 
              std::ostream_iterator<float>(out, "\n"));
  }
private:
  int index(int i, int j, int k) const
  { return i + j * m_sizeX + k * m_sizeX * m_sizeY; }
  int m_sizeX, m_sizeY, m_sizeZ;
  float *m_localToWorld;
  std::vector<float> m_data;
};

int main()
{
  float localToWorld[16];
  VoxelBuffer buffer(10, 10, 10, localToWorld);
  buffer.write("test.txt");
  VoxelBuffer bufferToRead(1, 1, 1, localToWorld);
  bufferToRead.read("test.txt");
  bufferToRead.write("test2.txt");
}
