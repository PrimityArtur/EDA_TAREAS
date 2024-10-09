import json
import matplotlib.pyplot as plt
import matplotlib.patches as patches

def draw_node(ax, node):
    for branch in node['branches']:
        rect = branch['rect']
        min_x, min_y = rect['min']
        max_x, max_y = rect['max']
        width = max_x - min_x
        height = max_y - min_y

        # Dibujar el rectngulo
        rect_patch = patches.Rectangle((min_x, min_y), width, height, linewidth=1, edgecolor='green', facecolor='none')
        ax.add_patch(rect_patch)

        if 'child' in branch:
            draw_node(ax, branch['child'])
        else:
            # dibujar los puntos
            data = branch.get('data', [])
            x_points = [point[0] for point in data]
            y_points = [point[1] for point in data]
            ax.scatter(x_points, y_points, c='black', s=10, marker='*')

            # unir puntos
            if len(data) > 1:
                '''
                x_points.append(data[0][0])
                y_points.append(data[0][1])
                '''
                ax.plot(x_points, y_points, c='blue', linewidth=0.5)            
            '''
            '''

def main():

    with open('json_rtree.json', 'r') as f:
        data = json.load(f)

    fig, ax = plt.subplots()
    # dibujar el MBR de root
    rect = data['rect']
    min_x, min_y = rect['min']
    max_x, max_y = rect['max']
    width = max_x - min_x
    height = max_y - min_y
    rect_patch = patches.Rectangle((min_x, min_y), width, height, linewidth=0.5, edgecolor='red', facecolor='none')
    ax.add_patch(rect_patch)
    # dibujar los nodos
    draw_node(ax, data['root'])


    # Cnfigurar labels
    ax.set_xlabel('Eje X')
    ax.set_ylabel('Eje Y')
    ax.set_title('Grafica del R-tree')
    ax.set_aspect('equal', adjustable='box')

    plt.show()

if __name__ == "__main__":
    main()

