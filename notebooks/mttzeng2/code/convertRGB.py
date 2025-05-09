import base64
from PIL import Image
import numpy as np

# STEP 1: Paste your base64 image string here
base64_data = """
""".replace("\n", "").replace(" ", "")
base64_data += '=' * ((4 - len(base64_data) % 4) % 4)

# STEP 2: Decode and byte-swap
raw = bytearray(base64.b64decode(base64_data))

# Byte-swap every 2 bytes (LSB <-> MSB)
swapped = bytearray()
for i in range(0, len(raw), 2):
    if i + 1 < len(raw):  # prevent IndexError on odd length
        swapped.append(raw[i+1])
        swapped.append(raw[i])

# Step 4: Convert to RGB image
w, h = 320, 240  # QVGA resolution
if len(swapped) != w * h * 2:
    raise ValueError(f"Data length mismatch: expected {w*h*2}, got {len(swapped)} bytes")

arr = np.frombuffer(swapped, dtype='>u2').reshape((h, w))

def rgb565_to_rgb888(p):
    r = ((p >> 11) & 0x1F) << 3
    g = ((p >> 5) & 0x3F) << 2
    b = (p & 0x1F) << 3
    return (r, g, b)

rgb_array = np.array([[rgb565_to_rgb888(px) for px in row] for row in arr], dtype=np.uint8)
img = Image.fromarray(rgb_array, 'RGB')
img.save("frame_qvga_fixed3.png")
print("Saved as frame_qvga_fixed3.png")