from random import choice

class Grafo:
    def __init__(self):
        self.vertices = {}
        self.cantidad = 0

    def pertenece(self, id):
        return id in self.vertices

    def agregar_vertice(self, id):  
        if not self.pertenece(id):
            self.vertices[id] = set()
            self.cantidad += 1
            return True
        return False

    def agregar_arista(self, id_vert_1, id_vert_2):
        if self.pertenece(id_vert_1):
            self.vertices[id_vert_1].add(id_vert_2)
            if not self.pertenece(id_vert_2):
                self.agregar_vertice(id_vert_2)
            return True
        return False

    def eliminar_arista(self, id_vert_1, id_vert_2):
        if self.pertenece(id_vert_1):
            self.vertices[id_vert_1].discard(id_vert_2)
            return True
        return False
        
    def borrar_vertice(self, id):
        if self.pertenece(id):
            vertice_borrado = self.vertices.pop(id)
            for v in self.vertices:
                if v.es_vecino(id):
                    v.eliminar_arista(id)
            self.cantidad -= 1
            return vertice_borrado
        return None

    def es_vecino(self, id_vert_1, id_vert_2):
        return self.pertenece(id_vert_1) and id_vert_2 in self.vertices[id_vert_1]
    
    def vertice_aleatorio(self):
        return choice(tuple(self.vertices.keys()))
    
    def obtener_vertices(self):
        return set(self.vertices)
    
    def obtener_vecinos(self, id):
        if self.pertenece(id):
            return self.vertices[id]
        return None

    def obtener_padres(self, id):
        if self.pertenece(id):
            return self.vertices[id].obtener_padres()
        return None

    def obtener_cantidad(self):
        return self.cantidad

    def vecino_aleatorio(self, id):
        if self.pertenece(id):
            return choice(list(self.vertices[id]))
    
    def cantidad_vecinos(self, id):
        if self.pertenece(id):
            return len(self.vertices[id])
        return None
    
    def iterar_grafo(self, visitar, extra):
        for i in self.vertices:
            if not visitar(self, i, extra):
                return False
        return True
