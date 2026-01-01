import numpy as np
from matplotlib import pyplot as plt

def plot_4_tiles(left_top, right_top, left_bottom, right_bottom, title="Tile Mosaic"):
    top = np.hstack([left_top, right_top])
    bottom = np.hstack([left_bottom, right_bottom])
    mosaic = np.vstack([top, bottom])

    plt.figure(figsize=(8, 8))
    plt.imshow(mosaic, cmap="terrain")
    plt.colorbar(label="value")
    plt.title(title)
    plt.axis("on")
    plt.show()