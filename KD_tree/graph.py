import sys
import json
import matplotlib.pyplot as plt
import os
import re

# verificar que se paso el nombre del archivo
if len(sys.argv) != 2:
    print("Uso: python tu_programa.py nombre_del_archivo.json")
    sys.exit(1)

json_filename = sys.argv[1]

# extraer nombre
base_filename = os.path.basename(json_filename)

# quitar extesnsion
filename_no_ext = os.path.splitext(base_filename)[0]

# separar letra del num
match = re.match(r'^([fk])(\d+)$', filename_no_ext)

algorithm_code, iteration_str = match.groups()
iteration = int(iteration_str)

algorithm_map = {
    'f': 'fuerza bruta',
    'k': 'kd-tree'
}

algorithm = algorithm_map.get(algorithm_code)

with open(json_filename) as f:
    data = json.load(f)

# verificar estructura
if "clusters" not in data:
    print("El archivo JSON no contiene la clave 'clusters'.")
    sys.exit(1)
    
num_clusters = len(data["clusters"])

# tab-20 para 20 colores
cmap = plt.get_cmap('tab20')

#  lista de colores a partir del colormap
colors = cmap.colors if hasattr(cmap, 'colors') else [cmap(i) for i in range(cmap.N)]

# asignar color a cluster
for idx, (cluster_id, cluster) in enumerate(data["clusters"].items()):
    centroid, points = cluster
    points = [tuple(point) for point in points]
    color = colors[idx % len(colors)]
    plt.scatter(*zip(*points), label=f"Cluster {cluster_id}", s=10, color=color)
    plt.scatter(*centroid, color="black", marker="x", s=50)

title = f"Clusters del algoritmo k-means con {algorithm} en la iteración = {iteration}"
plt.title(title)

#plt.legend()
plt.xlabel('X') 
plt.ylabel('Y')            
plt.show()