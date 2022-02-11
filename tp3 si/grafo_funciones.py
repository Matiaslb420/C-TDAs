from random import random
from collections import deque

VUELTAS_PAGE_RANK = 5

#funciones de visitados

def label_prop(grafo,vertice_act,etiquetas):
    repeticiones = {}
    mas_repetido = [0,0]
    for p in grafo.obtener_vecinos(vertice_act):
        try:
            repeticiones[etiquetas[p]] += 1
        except KeyError:
            repeticiones[etiquetas[p]] = 1
        if repeticiones[etiquetas[p]] > mas_repetido[1]:
            mas_repetido[1] = repeticiones[etiquetas[p]]
            mas_repetido[0] = etiquetas[p]
    etiquetas[vertice_act] = mas_repetido[0]
    return True

def calcular_cfc(grafo, vertice_actual, visitados, apilados, salida, orden, mb, pila, orden_contador):
    visitados.add(int(vertice_actual))
    orden[vertice_actual] = orden_contador
    mb[vertice_actual] = orden_contador
    orden_contador += 1 
    pila.appendleft(vertice_actual)
    apilados.add(vertice_actual)
    for vecino in grafo.obtener_vecinos(vertice_actual):
        if vecino not in visitados:
            orden_contador = calcular_cfc(grafo, vecino, visitados, apilados, salida, orden, mb, pila, orden_contador)

        if vecino in apilados:
            mb[vertice_actual] = min(mb[vertice_actual], mb[vecino])

        if orden[vertice_actual] == mb[vertice_actual] and pila:
            nueva_cfc = []
            while pila:
                vecino = pila.popleft()
                apilados.remove(vecino)
                nueva_cfc.append(vecino)
                if vecino == vertice_actual:
                    break
            salida.append(nueva_cfc)
    return orden_contador

def cad_rumores(red_criminal ,actual, padres, orden, parametros):
    if orden[actual] > parametros[0]:
        return False
    parametros[1].add(actual)
    return True

def camino_min(grafo, actual, padres, orden, parametros):
    if actual in parametros[0]:
        parametros[1].append(actual)
        padre = padres[actual]
        while padre != None:
            parametros[1].append(padre)
            padre = padres[padre]
        return False
    return True

def actualizar_pr(grafo, vertice, page_rank):
    vecinos = grafo.obtener_vecinos(vertice)
    cant_vecinos_pr =  grafo.cantidad_vecinos(vertice) 
    for vecino in vecinos:
        page_rank[1][vecino] += page_rank[0][vertice] / cant_vecinos_pr
    if cant_vecinos_pr == 0:
        page_rank[1][vertice] /=  grafo.obtener_cantidad()
    return True

#funciones genericas

def page_rank(grafo, ranking_global):
    if not ranking_global[0]:
        page_rank = [{}, {}]
        p_r_inicial= 1 / grafo.obtener_cantidad()
        inicializado = {}
        for v in grafo.vertices:
            page_rank[0][v] = p_r_inicial
            inicializado[v] = 0
        page_rank[1] = inicializado.copy()
        for i in range(VUELTAS_PAGE_RANK):
            grafo.iterar_grafo(actualizar_pr, page_rank)
            page_rank[0] = page_rank[1].copy()
            page_rank[1] = inicializado.copy()
        ranking_global[0] = sorted(page_rank[0].items(), key = lambda x:x[1], reverse= True)

def random_walk(grafo, max_iter, visitar, coef_d, extra):
        vertice_i = grafo.vertice_aleatorio()
        i = 0
        while i < max_iter:
            if not visitar(grafo, vertice_i, extra):
                break
            if random() > coef_d:
                vertice_i = grafo.vertice_aleatorio()
            else:
                vertice_i = grafo.vecino_aleatorio(vertice_i)
            i += 1

def recur_dfs(grafo,vertice_act,visitados,visitar,extra):
    visitados.add(vertice_act)
    if visitar:
        if not visitar(grafo,vertice_act,extra):
            return False
    for v in grafo.obtener_vecinos(vertice_act):
        if v not in visitados:
            if not recur_dfs(grafo,v,visitados,visitar,extra):
                return False
    return True

def dfs(grafo,vertice_i,visitar,extra):
    visitados = set()
    recur_dfs(grafo,vertice_i,visitados,visitar,extra)

def bfs(grafo, origen, visitar, extra):
    visitados = {origen}
    padres = {origen:None}
    orden = {origen:0}
    cola_v = deque([origen], maxlen=None)
    while cola_v:
        actual = cola_v.popleft()
        vecinos = grafo.obtener_vecinos(actual)
        if visitar:
            if not visitar(grafo, actual, padres, orden, extra):
                break
        if not vecinos:
            continue
        for v in vecinos:
            if v not in visitados:
                visitados.add(v)
                padres[v] = actual
                orden[v] = orden[actual] + 1
                cola_v.append(v)
    return None

def ciclo_backtracking(grafo, vertice_actual, contador, destino, cant_ciclos, camino, visitados):
    vecinos = grafo.obtener_vecinos(vertice_actual)
    if contador == cant_ciclos - 1:
        if destino in vecinos:
            camino.append(destino)
            return True
        return False
    for vecino in vecinos:
        if vecino in visitados:
            continue
        camino.append(vecino)
        visitados.add(vecino)
        if ciclo_backtracking(grafo, vecino, contador + 1, destino, cant_ciclos, camino, visitados):
            return True
        camino.pop()
        visitados.remove(vecino)
    return False