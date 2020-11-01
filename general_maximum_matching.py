import Queue
import random
def gcd(a, b):
    if b == 0:
        return a
    else:
        return gcd(b, a % b)

def is_pow2_multiple(big, small):
    multiplier = 1
    for _ in range(31):
        if small * multiplier == big:
            return True
        multiplier *= 2
    return False

N = 0
G = [[]]
matching = [[]]
contracted = [False] * 100

class wedge:
    def __init__(self, vfrom, vto, value):
        self.vfrom = vfrom
        self.vto = vto
        self.value = value


def edmonds_blossom(): # I realized there's a way simpler solution :)
    global N, G, matching, contracted
    inF = [True] * N
    visited = [False] * N
    depth = [-1] * N
    parent = [-1] * N
    root = [-1] * N
    for v in range(N):
        if not contracted[v]:
            root[v] = v
            parent[v] = v
    for match in matching:
        if len(match) == 1:
            inF[match[0]] = False
    unmarked_edges = [[False for _ in range(N)] for _ in range(N)]
    for vfrom in range(N):
        for vto in range(N):
            if G[vfrom][vto] and ((len(matching[vfrom]) == 0) or matching[vfrom][0] != vto):
                unmarked_edges[vfrom][vto] = True
    bfs_q = Queue.Queue()
    for n in range(N):
        if inF[n] and not contracted[n]:
            bfs_q.put(n)
            depth[n] = 0
    while not bfs_q.empty():
        v = bfs_q.get()
        if visited[v]:
            continue
        visited[v] = True
        for w in range(N):
            if not unmarked_edges[v][w]:
                continue
            unmarked_edges[v][w] = False
            unmarked_edges[w][v] = False
            if not inF[w]:
                inF[w] = True
                root[w] = root[v]
                parent[w] = v
                depth[w] = depth[v] + 1
                x = matching[w][0]
                inF[x] = True
                root[x] = root[v]
                parent[x] = w
                depth[x] = depth[w] + 1
                bfs_q.put(x)
            elif depth[w] % 2 == 0:
                if root[v] != root[w]:
                    pos = v
                    path = [pos]
                    while pos != root[v]:
                        pos = parent[pos]
                        path.append(pos)
                    pos = w
                    path.append(pos)
                    while pos != root[w]:
                        pos = parent[pos]
                        path.append(pos)
                    return path
                else:
                    # Create blossom
                    in_blossom = [False] * N
                    pos1 = v
                    pos2 = w
                    p1parents = []
                    p2parents = []
                    while pos1 != pos2:
                        if depth[pos1] > depth[pos2]:
                            contracted[pos1] = True
                            p1parents.append(pos1)
                            in_blossom[pos1] = True
                            pos1 = parent[pos1]
                        else:
                            contracted[pos2] = True
                            p2parents.append(pos2)
                            in_blossom[pos2] = True
                            pos2 = parent[pos2]
                    merged_node = pos1
                    in_blossom[merged_node] = True
                    p1parents.reverse()
                    blossom_cycle = p1parents
                    for p2par in p2parents:
                        blossom_cycle.append(p2par)
                    
                    # Conract blossom
                    changed_edges = []
                    for pos in blossom_cycle:
                        for otherv in range(N):
                            if G[pos][otherv]:
                                value = G[pos][otherv]
                                changed_edges.append(wedge(pos, otherv, value))
                                G[pos][otherv] -= value
                                G[otherv][pos] -= value
                                if not in_blossom[otherv] and otherv != merged_node:
                                    G[merged_node][otherv] += value
                                    G[otherv][merged_node] += value

                    # Find path in contracted graph
                    path = edmonds_blossom()
                    if len(path) == 0:
                        return path

                    # Add edges back
                    for pos in blossom_cycle:
                        contracted[pos] = False
                    for wed in changed_edges:
                        vfrom = wed.vfrom
                        vto = wed.vto
                        value = wed.value
                        G[vfrom][vto] += value
                        G[vto][vfrom] += value
                    if not in_blossom[vto] and vto != merged_node:
                        G[merged_node][vto] -= value
                        G[vto][merged_node] -= value
                    
                    # Lift 
                    goal_out = -1
                    rev = False
                    augmented_path = []
                    for p_i in range(len(path)):
                        augmented_path.append(path[p_i])
                        if p_i < len(path) - 1:
                            if path[p_i] == merged_node and not G[merged_node][path[p_i + 1]]:
                                goal_out = path[p_i + 1]
                                break
                            elif not G[path[p_i]][path[p_i + 1]]:
                                rev = True
                                break
                    if rev:
                        path.reverse()
                        augmented_path = []
                        for p_i in range(len(path)):
                            augmented_path.append(path[p_i])
                            if p_i < len(path) - 1:
                                if path[p_i] == merged_node and not G[merged_node][path[p_i + 1]]:
                                    goal_out = path[p_i + 1]
                                    break
                    
                    if goal_out != -1:
                        goal_in = -1
                        for bl in blossom_cycle:
                            if G[goal_out][bl]:
                                goal_in = bl
                                break
                        aug1 = []
                        aug2 = []
                        goal_reached = False
                        for pos in blossom_cycle:
                            if pos == goal_in:
                                goal_reached = True
                                aug1.append(goal_in)
                                aug2.append(goal_in)
                            elif not goal_reached:
                                aug1.append(pos)
                            else:
                                aug2.append(pos)
                        aug2.reverse()
                        if len(aug1) % 2 == 0:
                            for augvert in aug1:
                                augmented_path.append(augvert)
                        else:
                            for augvert in aug2:
                                augmented_path.append(augvert)
                    goal_passed = False
                    for pathvert in path:
                        if pathvert == goal_out:
                            goal_passed = True
                        if goal_passed:
                            augmented_path.append(pathvert)
                    return augmented_path
    return []

def maxmatching():
    global matching, contracted
    n_matched = 0
    matching = [[] for _ in range(N)]
    contracted = [False] * N
    while(True):
        path = edmonds_blossom()
        if len(path) == 0:
            break
        else:
            n_matched += 2
            for p_i in range(0, len(path) - 1, 2):
                v1 = path[p_i]
                v2 = path[p_i + 1]
                matching[v1] = [v2]
                matching[v2] = [v1]
    return n_matched

def solution(banana_list):
    global N, G, matching, contracted
    N = len(banana_list)
    G = [[0 for _ in range(N)] for _ in range(N)] # adjacency matrix   
    # Build graph, where edges are the pairs (i, j) with infinite game length
    for i in range (N - 1):
        for j in range(i + 1, N):
            ij_gcd = gcd(banana_list[i], banana_list[j])
            sum = banana_list[i] + banana_list[j]
            if not is_pow2_multiple(sum, ij_gcd): # infinite game between i and j
                G[i][j] = 1
                G[j][i] = 1
    return N - maxmatching()
