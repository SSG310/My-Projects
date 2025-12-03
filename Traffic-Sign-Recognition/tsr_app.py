import streamlit as st
import tensorflow as tf
import cv2
import numpy as np
from PIL import Image
import os

# Load the model
model_path = r"D:\ML_proj(1)\model\model.keras"  # Model path (adjust if needed)
if os.path.exists(model_path):
    model = tf.keras.models.load_model(model_path)
else:
    st.error("Model file not found.")
    exit()

# Define the classes for traffic signs
classes = {0: 'Speed limit (20km/h)', 1: 'Speed limit (30km/h)', 2: 'Speed limit (50km/h)',
           3: 'Speed limit (60km/h)', 4: 'Speed limit (70km/h)', 5: 'Speed limit (80km/h)',
           6: 'End of speed limit (80km/h)', 7: 'Speed limit (100km/h)', 8: 'Speed limit (120km/h)',
           9: 'No passing', 10: 'No passing veh over 3.5 tons', 11: 'Right-of-way at intersection',
           12: 'Priority road', 13: 'Yield', 14: 'Stop', 15: 'No vehicles', 16: 'Veh > 3.5 tons prohibited',
           17: 'No entry', 18: 'General caution', 19: 'Dangerous curve left', 20: 'Dangerous curve right',
           21: 'Double curve', 22: 'Bumpy road', 23: 'Slippery road', 24: 'Road narrows on the right',
           25: 'Road work', 26: 'Traffic signals', 27: 'Pedestrians', 28: 'Children crossing',
           29: 'Bicycles crossing', 30: 'Beware of ice/snow', 31: 'Wild animals crossing',
           32: 'End speed + passing limits', 33: 'Turn right ahead', 34: 'Turn left ahead',
           35: 'Ahead only', 36: 'Go straight or right', 37: 'Go straight or left', 38: 'Keep right',
           39: 'Keep left', 40: 'Roundabout mandatory', 41: 'End of no passing',
           42: 'End no passing veh > 3.5 tons'}

# Streamlit app layout
st.title("Traffic Sign Recognition")
st.write("Upload an image of a traffic sign and get the classification result.")

# File uploader widget
uploaded_file = st.file_uploader("Choose a traffic sign image", type=["jpg", "png", "jpeg"])

# Function to preprocess image and predict class
def predict_traffic_sign(image):
    IMG_HEIGHT, IMG_WIDTH = 30, 30
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)  # Convert image to RGB
    image_fromarray = Image.fromarray(image, 'RGB')
    resize_image = image_fromarray.resize((IMG_HEIGHT, IMG_WIDTH))
    image_array = np.array(resize_image)
    image_array = np.expand_dims(image_array, axis=0)  # Add batch dimension
    image_array = image_array / 255.0  # Normalize

    # Predict using the loaded model
    prediction = model.predict(image_array)
    predicted_class = np.argmax(prediction, axis=1)[0]
    return predicted_class

# If an image is uploaded, display the image and classification result
if uploaded_file is not None:
    # Open and display the uploaded image
    image = np.array(Image.open(uploaded_file))
    st.image(image, caption='Uploaded Image', use_container_width=True)

    #st.image(image, caption="Uploaded Image", use_column_width=True)

    # Predict the traffic sign class
    predicted_class = predict_traffic_sign(image)

    # Display the predicted class
    st.write(f"Predicted Traffic Sign: {classes[predicted_class]}")

    # Optionally, show the model's confidence
    
    #prediction_prob = model.predict(np.expand_dims(image, axis=0) / 255.0)
    #st.write(f"Confidence: {np.max(prediction_prob) * 100:.2f}%")

