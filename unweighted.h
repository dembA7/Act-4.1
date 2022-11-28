// =================================================================
//
// File: unweighted.h
// Author: Arturo Díaz 
// Date: 09/11/22
//
// =================================================================

#ifndef UNWEIGHTED_H
#define UNWEIGHTED_H

#include <vector>
#include <iostream>
#include <set>
#include <stack>

// =================================================================
// Definition of the Graph class
// =================================================================

template <class Vertex>
class Graph{
public:
  virtual std::vector<Vertex> getVertexes() const = 0;
  virtual std::set<Vertex> getNeighbours(Vertex) const = 0;
  virtual void addVertex(Vertex) = 0;
  virtual void addEdge(Vertex, Vertex) = 0;
};

// =================================================================
// Definition of the Unweighted class
// =================================================================

template <class Vertex>
class Unweighted : public Graph<Vertex>{
private:
  int size, next;
  std::vector<std::vector<bool>> matrix;
  std::vector<Vertex> vertexes;
  int indexOf(Vertex) const;

public:
  Unweighted();
  Unweighted(int);

  std::vector<Vertex> getVertexes() const;
  std::set<Vertex> getNeighbours(Vertex) const;

  void addVertex(Vertex);
  void addEdge(Vertex, Vertex);

  std::vector<Vertex> DFS(Vertex, const Graph<Vertex> *);

  template <class T>
  friend std::ostream &operator<<(std::ostream &, const Unweighted<T> &);
};

// =================================================================
// Constructor por default
// =================================================================

template <class Vertex>
Unweighted<Vertex>::Unweighted(){
  next = 0;
  size = 0;
}

// =================================================================
// Constructor
// =================================================================

template <class Vertex>
Unweighted<Vertex>::Unweighted(int size){
  matrix = std::vector<std::vector<bool>>(size, std::vector<bool>(size, false));
  vertexes = std::vector<Vertex>(size);
  next = 0;
}

// =================================================================
// Función que regresa todos los vértices
// =================================================================

template <class Vertex>
std::vector<Vertex> Unweighted<Vertex>::getVertexes() const{
  return vertexes;
}

// =================================================================
// Función que obtiene el índice de un vértice
// =================================================================

template <class Vertex>
int Unweighted<Vertex>::indexOf(Vertex vertex) const{
  for (int i = 0; i < vertexes.size(); i++)
    if (vertexes[i] == vertex)
      return i;

  return -1;
}

// =================================================================
// Función que regresa los vecinos de un vértice
// =================================================================

template <class Vertex>
std::set<Vertex> Unweighted<Vertex>::getNeighbours(Vertex vertex) const{
  std::set<Vertex> neighbours;
  int index = indexOf(vertex);
  for (int i = 0; i < next; i++)
    if (index != i && matrix[index][i])
      neighbours.insert(vertexes[i]);

  return neighbours;
}

// =================================================================
// Función que agrega un vértice
// =================================================================

template <class Vertex>
void Unweighted<Vertex>::addVertex(Vertex vertex){
  if (indexOf(vertex) == -1 && next != size){
    vertexes[next] = vertex;
    next++;
  }
}

// =================================================================
// Función que agrega un edge
// =================================================================

template <class Vertex>
void Unweighted<Vertex>::addEdge(Vertex fromVertex, Vertex toVertex){
  int fromIndex = indexOf(fromVertex);
  if (fromIndex == -1){
    if (this->next == this->size)
      throw std::out_of_range("No hay espacio para agregar el vértice");
    vertexes.push_back(fromVertex);
    next++;
  }

  int toIndex = indexOf(toVertex);
  if (toIndex == -1){
    if (this->next == this->size)
      throw std::out_of_range("No hay espacio para agregar el vértice");
    vertexes.push_back(toVertex);
    next++;
  }

  matrix[fromIndex][toIndex] = true;
  matrix[toIndex][fromIndex] = true;
}

// =================================================================
// Operator overload
// =================================================================

template <class T>
std::ostream &operator<<(std::ostream &os, const Unweighted<T> &graph){
  os << "Vértices: ";
  for (int i = 0; i < graph.vertexes.size(); i++)
    os << graph.vertexes[i] << " ";
  os << std::endl;

  os << "\nMatriz de adyacencia: " << std::endl;
  for (int i = 0; i < graph.vertexes.size(); i++){
    for (int j = 0; j < graph.vertexes.size(); j++)
      os << graph.matrix[i][j] << " ";
    os << std::endl;
  }

  return os;
}

// =================================================================
// Función que recorre el grafo
// =================================================================

template <class Vertex>
std::vector<Vertex> Unweighted<Vertex>::DFS(Vertex start, const Graph<Vertex> *graph){
  std::set<Vertex> visited;
  std::vector<Vertex> vertexes;
  std::stack<Vertex> pending;
  pending.push(start);

  while (!pending.empty()){
    Vertex current = pending.top();
    pending.pop();

    if (visited.find(current) == visited.end()){
      visited.insert(current);
      vertexes.push_back(current);
      std::set<Vertex> neighbours = graph->getNeighbours(current);

      for (auto neighbour : neighbours)
        pending.push(neighbour);
    }
  }

  return vertexes;
}

#endif