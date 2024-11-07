import sys
import json
import matplotlib.pyplot as plt
import math

if len(sys.argv) != 4:
    print("Uso: python script.py nombre_del_archivo.json nombre_axiX constant")
    sys.exit(1)

json_filename = sys.argv[1]
axiName_x = sys.argv[2]
constantName = sys.argv[3]

with open(json_filename) as f:
    data = json.load(f)

# cantidad de graficos y ubicacion de subplots
num_plots = len(data)
cols = min(num_plots, 3)  # Máximo 3 gráficos por fila, ajustable
rows = math.ceil(num_plots / cols)

# crear subplots
fig, axes = plt.subplots(rows, cols, figsize=(5 * cols, 4 * rows))
axes = axes.flatten()  # Aplanar para iterar si hay una fila

# Graficar cada constant
for idx, entry in enumerate(data):
    constant = entry["constant"]
    times_kmeans = entry["times_Kmeans"]

    # Extraer datos "Brute Force" "KD-Tree"
    x_values = sorted(map(int, times_kmeans.keys()))
    brute_force_y = [point["Brute Force"] for x in x_values for point in times_kmeans[str(x)]]
    kd_tree_y = [point["KD-Tree"] for x in x_values for point in times_kmeans[str(x)]]

    # Graficar puntos 
    axes[idx].scatter(x_values, brute_force_y, label="Brute Force", s=10, color="blue")
    axes[idx].plot(x_values, brute_force_y, color="blue", linestyle="--", linewidth=1)
    axes[idx].scatter(x_values, kd_tree_y, label="KD-Tree", s=10, color="orange")
    axes[idx].plot(x_values, kd_tree_y, color="orange", linestyle="--", linewidth=1)

    axes[idx].set_title(f"{constantName} == {constant}")
    axes[idx].set_xlabel(axiName_x)
    axes[idx].set_ylabel("Running Time (ms)")
    axes[idx].legend()
    axes[idx].grid(True)

plt.tight_layout()
plt.show()
