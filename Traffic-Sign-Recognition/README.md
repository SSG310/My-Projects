# 🚦 Traffic Sign Detection and Classification Using CNN  
A deep-learning-based system for classifying traffic signs using a custom-built Convolutional Neural Network (CNN) trained on the **GTSRB dataset**.  
This project demonstrates high-accuracy traffic sign classification and includes GUI-based prediction functionality.  

---

## 📌 Overview  
Traffic Sign Recognition (TSR) is critical in modern transportation systems, advanced driver assistance systems (ADAS), and autonomous vehicles. It enhances road safety by identifying regulatory, warning, and informational signs in real time.

This project uses a **10-layer CNN architecture** to classify 43 different traffic sign classes from the **German Traffic Sign Recognition Benchmark (GTSRB)** dataset.  

✔ Achieved **98.8%** test accuracy in experimentation
✔ Achieved **99% accuracy for static GUI-based predictions**

---

## 🗂 Dataset  
The model uses the **GTSRB dataset**, containing 43 classes of real-world traffic signs.  
According to *Figure 1 (Page 4)*, the dataset features a balanced distribution of traffic sign categories.  

Key steps:
- Resize images to **30×30×3** pixels  
- Normalize pixel values to [0,1]  
- Shuffle dataset for randomness  
---

## 🧠 Data Augmentation  
To overcome class imbalance and improve robustness, the model applies augmentations such as:  
- Rotation (10°)  
- Zoom (15%)  
- Width/height shift (10%)  
- Shear (15%)  
- Nearest fill mode  

---

## 🏗 Model Architecture  
The proposed architecture consists of:  
- **4 Convolutional Layers** (filters: 16, 32, 64, 128)  
- **2 Max Pooling Layers**  
- **3 Batch Normalization Layers**  
- **1 Flatten Layer**  
- **1 Dense Layer (512 neurons)**  
- **Dropout (0.5)**  
- **Softmax Output Layer (43 classes)**  

---

## ⚙ Training Configuration  
The model uses: 

- **Loss function:** Categorical Crossentropy  
- **Optimizer:** Adam (LR = 0.001 with decay)  
- **Batch size:** 32  
- **Epochs:** 30  
- **Activation functions:** ReLU, Softmax  
- **Validation split:** 30%  

Training includes accuracy monitoring and validation tuning

---

## 🧪 Testing & Evaluation  
Evaluation includes:  
- Accuracy measurement  
- Confusion matrix  
- Per-class precision, recall, F1-score  

A full classification report is shown in :  
✔ Weighted accuracy: **99%**  
✔ Macro average: **98%**  

---

## 📊 Results  
According to the *Results Section : 

- Achieved **99% accuracy** for static image predictions  
- Strong robustness across lighting, weather variations, and partial occlusions  
- Accurate classification across 43 categories  
- Demonstrated potential for real-time use


---

## 🚀 Conclusion  
The CNN-based TSR system provides a **highly accurate, reliable, and practical solution** for traffic sign classification.  
It addresses natural variations such as weather conditions, lighting, and partial occlusion, making it suitable for deployment in intelligent transportation systems.  

---

## 🔮 Possible Future Enhancements  
Future improvements include:  
- Using attention-based models (Vision Transformers, hybrid CNN-ViT)  
- Improving real-time adaptability  
- Supporting datasets from multiple countries  
- Deploying on lightweight devices (Raspberry Pi, mobile)  
- Using better augmentation techniques  
- Improving security against adversarial attacks  
- Adding multi-sensor support (GPS, LiDAR)  

---

## 👥 Team  
- **Snehanshu Gunjal**
- Hiba Ashekhan 
- Sourabh Pawar 
- Suchitkumar Khadakabhavi


Guides: **Dr. Arun Tigadi, Prof. Anita Shirol**  



