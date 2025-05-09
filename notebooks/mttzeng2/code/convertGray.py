import base64
from PIL import Image
import numpy as np

# STEP 1: Read base64 string from a file
with open("gray_text.txt", "r") as f:
    base64_data = f.read().replace("\n", "").replace(" ", "")
    base64_data += '=' * ((4 - len(base64_data) % 4) % 4)  # pad if necessary

# STEP 2: Decode base64 to raw grayscale bytes
raw = bytearray(base64.b64decode(base64_data))

# STEP 3: Define image dimensions
w, h = 320, 240  # QVGA resolution (adjust if needed)

# STEP 4: Check length
if len(raw) != w * h:
    raise ValueError(f"Data length mismatch: expected {w*h} bytes, got {len(raw)} bytes")

# STEP 5: Convert to grayscale image
arr = np.frombuffer(raw, dtype=np.uint8).reshape((h, w))
img = Image.fromarray(arr, 'L')  # 'L' = 8-bit pixels, black and white

# STEP 6: Save image
img.save("demo.png")
# print("Saved as frame_qvga_grayscale.png")