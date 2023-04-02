import sys
import numpy as np

NORMAL_SCALE = 4096
VERTEX_SCALE = 100

def get_normal(v1, v2, v3):
    
    # Calculate two vectors that lie on the plane of the face
    vector1 = np.array(v2) - np.array(v1)
    vector2 = np.array(v3) - np.array(v1)
    
    # Calculate the cross product of the two vectors
    normal = np.cross(vector1, vector2)
    
    # Normalize the resulting vector
    normal = normal / np.linalg.norm(normal) * NORMAL_SCALE
    normal = list(normal)
    
    return list(normal)

vertices = []
indices = []
normals = []

if len(sys.argv) != 2:
    print(sys.argv)
    print("usage: main.py objfile")
    sys.exit()

file_name = sys.argv[1]

with open(file_name, 'r') as file:

    for line in file:
        
        if line.startswith('v '):
            vertex = list(map(float, line[2:].split()))
            vertices.append(vertex)

        elif line.startswith('f '):

            # extract vertex indices from line containing a face 
            face_indices = list(map(lambda s: int( s.split("/")[0] )-1, line[2:].split()))
            indices.append(face_indices)

            v1 = vertices[face_indices[0]]
            v2 = vertices[face_indices[1]]
            v3 = vertices[face_indices[2]]

            normals.append(get_normal(v1, v2, v3))

# Scale vertices to be easily visible in the psx engine
vertices = list(map(lambda v: [VERTEX_SCALE*v[0], VERTEX_SCALE*v[1], VERTEX_SCALE*v[2]], vertices))

nfaces = len(indices)

# Reorder indices so they are rendered correctly
indices = list(map(lambda i: [ i[3], i[2], i[0], i[1] ], indices))

# convert lists to strings representing c arrays of structs
vertices = str(vertices).replace("[", "{").replace("]", "}")
indices = str(indices).replace("[", "{").replace("]", "}")
normals = str(normals).replace("[", "{").replace("]", "}")

print('SVECTOR car_verts[] =', vertices, ";")
print('INDEX car_indices[] =', indices, ";")
print('SVECTOR car_norms[] =', normals, ";")
print('#define CAR_FACES', nfaces)
