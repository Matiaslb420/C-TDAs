#!/usr/bin/python3
import sys
import grafo
import grafo_funciones
from collections import deque

sys.setrecursionlimit(10**6)

def crear_grafo(archivo):
    datos_f = open(archivo, "r")
    red_criminal = grafo.Grafo()
    elemento_ant = -1
    for linea in datos_f:
        elementos = linea.rstrip("\n").split("\t")
        if elemento_ant != elementos[0]:
            elemento_ant = elementos[0]
            red_criminal.agregar_vertice(int(elemento_ant))
        red_criminal.agregar_arista(int(elementos[0]), int(elementos[1]))
    datos_f.close()
    return red_criminal

def llamar_comando(red_criminal,comando, page_rank):
    parametros = comando.rstrip('\n').split(' ')
    if parametros[0] == "min_seguimientos":
        min_seguimientos(red_criminal,int(parametros[1]),int(parametros[2]))
    elif parametros[0] == "mas_imp":
        mas_imp(red_criminal, int(parametros[1]), page_rank)
    elif parametros[0] == "persecucion":
        persecucion(red_criminal, parametros[1], int(parametros[2]), page_rank)
    elif parametros[0] == "comunidades":
        comunidades(red_criminal,int(parametros[1]))
    elif parametros[0] == "divulgar":
        divulgar(red_criminal, int(parametros[1]), int(parametros[2]))
    elif parametros[0] == "divulgar_ciclo":
        divulgar_ciclo(red_criminal, int(parametros[1]), int(parametros[2]))
    elif parametros[0] == "cfc":
        cfc(red_criminal)
    else:
        print(red_criminal.vertice_aleatorio())
        print(f"comando invalido")
        print(f"comandos disponibles:")
        print(f"min_seguimientos")
        print(f"mas_imp")
        print(f"persecucion")
        print(f"comunidades")
        print(f"divulgar")
        print(f"divulgar_ciclo")
        print(f"cfc")

def algopoli(archivo):
    red_criminal = crear_grafo(archivo)
    comando = input()
    page_rank = [[]]
    while(comando):
        llamar_comando(red_criminal,comando, page_rank)
        try:
            comando = input()
        except EOFError:
            break
    del red_criminal

def mas_imp(red_criminal,cantidad, page_rank):
    grafo_funciones.page_rank(red_criminal, page_rank)
    print(", ".join([str(i[0]) for i in page_rank[0][:cantidad]]))

def persecucion(red_criminal, informantes, cant_mas_importantes,  page_rank):
    informantes = informantes.split(',')
    if not page_rank[0]:
        grafo_funciones.page_rank(red_criminal, page_rank)
    ranking = page_rank[0][:cant_mas_importantes]
    longitud_menor = float("inf")
    menor_camino = []
    parametros = [[i[0] for i in ranking], []]
    for info in informantes:
        grafo_funciones.bfs(red_criminal, int(info), grafo_funciones.camino_min, parametros)
        if not parametros[1]:
            continue
        longitud_actual = len(parametros[1])
        if longitud_actual < longitud_menor:
            menor_camino = parametros[1].copy()
            longitud_menor = longitud_actual
    print(" -> ".join(list(reversed([str(i) for i in menor_camino]))))
        
def cfc(red_criminal):
    visitados = set()
    apilados = set()
    salida = []
    orden = {}
    mb = {}
    pila = deque([])
    orden_contador = 0
    delincuente = red_criminal.vertice_aleatorio()
    grafo_funciones.calcular_cfc(red_criminal, delincuente, visitados, apilados, salida, orden, mb, pila, orden_contador)
    contador = 1
    for cfc in salida:
        print("CFC", str(contador) + ':', ", ".join([str(i) for i in cfc]))
        contador += 1

def divulgar_ciclo(red_criminal, delincuente, n):
    contador = 0
    destino = delincuente
    cant_ciclos = n
    camino = [delincuente]
    visitados = {delincuente}
    if n == 0 or not grafo_funciones.ciclo_backtracking(red_criminal,delincuente, contador, destino, cant_ciclos, camino, visitados):
        print("No se encontro recorrido")
        return
    print(" -> ".join([str(i) for i in camino]))
       
def divulgar(red_criminal,delincuente, n):
    parametros = [n, set()]
    grafo_funciones.bfs(red_criminal, delincuente, grafo_funciones.cad_rumores, parametros)
    parametros[1].remove(delincuente)
    print(", ".join([str(i) for i in parametros[1]]))


def comunidades(red_criminal, inter_min):
    etiquetas = {}
    indice = 0
    comunidades = {}
    for vertice in red_criminal.obtener_vertices():
        etiquetas[vertice] = indice
        comunidades[indice] =[]
        indice += 1
    for i in range(200):
        grafo_funciones.dfs(red_criminal,red_criminal.vertice_aleatorio(),grafo_funciones.label_prop,etiquetas)
    for v in red_criminal.obtener_vertices():
        comunidades[etiquetas[v]].append(v)
    contador = 1
    for comunidad in comunidades.values():
        if len(comunidad) >= inter_min:
            print(f"comunidad {contador}:", ", ".join([str(i) for i in comunidad]))
            contador += 1

def min_seguimientos(red_criminal, origen, destino):
    if not (red_criminal.pertenece(origen) and red_criminal.pertenece(destino)):
        print("Inexistente")
        return None
    parametros = [[destino], []]
    grafo_funciones.bfs(red_criminal, origen, grafo_funciones.camino_min, parametros)
    if not parametros[1]:
        print("Seguimiento imposible")
        return
    print(" -> ".join(list(reversed([str(i) for i in parametros[1]]))))

algopoli(sys.argv[1])

