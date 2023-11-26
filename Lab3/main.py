import numpy as np
import cv2 as cv
import imutils
from tkinter import filedialog
from tkinter import ttk
from tkinter import *
from scipy import ndimage as ndi
from skimage.filters import edges
from skimage.feature import peak_local_max
from skimage.segmentation import watershed
from PIL import ImageTk, Image
from copy import deepcopy


class MainSolution():
    def __init__(self):
        path = filedialog.askopenfilename(filetypes=(
        ("PNG", "*.png"), ("JPG", "*.jpg"), ("GIF", "*.gif"), ("TIF", "*.tif"), ("BMP", "*.bmp"), ("PCX", "*.pcx")))
        self.image = cv.imread(path)
        self.imgray = None
        self.trsh1 = None
        self.trsh2 = None

    def filt(self):
        filtered_image = cv.pyrMeanShiftFiltering(self.image, 15, 50)
        self.imgray = cv.cvtColor(filtered_image, cv.COLOR_BGR2GRAY)
        img = Image.fromarray(self.imgray)
        img = img.resize((300, 300))
        return ImageTk.PhotoImage(img)

    def global_threshold(self):
        ret, thresh1 = cv.threshold(self.imgray, 0, 255, cv.THRESH_BINARY_INV | cv.THRESH_OTSU)
        self.trsh1 = deepcopy(thresh1)
        img = Image.fromarray(thresh1)
        img = img.resize((300, 300))
        return ImageTk.PhotoImage(img)

    def adaptive_threshold(self):
        thresh2 = cv.adaptiveThreshold(self.imgray, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C, cv.THRESH_BINARY, 11, 2)
        self.trsh2 = deepcopy(thresh2)
        img = Image.fromarray(thresh2)
        img = img.resize((300, 300))
        return ImageTk.PhotoImage(img)

    def segmentation(self):
        ret, thresh1 = cv.threshold(self.imgray, 0, 255, cv.THRESH_BINARY_INV | cv.THRESH_OTSU)

        kernel = np.ones((3, 3), np.uint8)
        sure_bg = cv.dilate(thresh1, kernel, iterations=3)
        dist_transform = cv.distanceTransform(thresh1, cv.DIST_L2, 5)
        ret, sure_fg = cv.threshold(dist_transform, 0.7 * dist_transform.max(), 255, 0)
        sure_fg = np.uint8(sure_fg)
        unknown = cv.subtract(sure_bg, sure_fg)

        ret, markers = cv.connectedComponents(sure_fg)
        markers += 1
        markers[unknown == 255] = 0

        markers = cv.watershed(self.image, markers)
        self.image[markers == -1] = [255, 0, 0]

        result_image = Image.fromarray(self.image)
        result_image = result_image.resize((300, 300))
        return ImageTk.PhotoImage(result_image)


root = Tk()
ms = MainSolution()
w, h = root.winfo_screenwidth(), root.winfo_screenheight()
root.geometry(f"700x700")

lbl_text1 = ttk.Label(text="Глобальная пороговая обработка")
lbl_text1.place(x=250, y=10)
img1 = ms.filt()
lbl1 = ttk.Label(image=img1)
lbl1.image = img1
lbl1.place(x=30, y=40, width=300, height=300)
img2 = ms.global_threshold()
lbl2 = ttk.Label(image=img2)
lbl2.image = img2
lbl2.place(x=370, y=40, width=300, height=300)

lbl_text2 = ttk.Label(text="Адаптивная пороговая обработка")
lbl_text2.place(x=90, y=360)
img3 = ms.adaptive_threshold()
lbl3 = ttk.Label(image=img3)
lbl3.image = img3
lbl3.place(x=30, y=390, width=300, height=300)

lbl_text3 = ttk.Label(text="Сегментация")
lbl_text3.place(x=480, y=360)
img4 = ms.segmentation()
lbl4 = ttk.Label(image=img4)
lbl4.image = img4
lbl4.place(x=370, y=390, width=300, height=300)

root.mainloop()
