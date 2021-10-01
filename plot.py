import matplotlib.pyplot as plt
import networkx as nx

def plot():
    plt.close()
    plt.ion()
    plt.show()
    
    G = nx.Graph()
    
    nx.draw(G, posG)
    
    plt.show()
    plt.pause(0)
    # plt.pause(0.001)

if __name__ == '__main__':
    plot()
