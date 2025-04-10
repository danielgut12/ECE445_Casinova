cv['onRuntimeInitialized'] = () => {
  console.log('OpenCV is ready');

  const video = document.getElementById('videoInput');
  const canvas = document.getElementById('canvasOutput');
  const suit = document.getElementById('suitDisplay');
  const rank = document.getElementById('rankDisplay');
  let streaming = false;


  // snags the camera connected to device (just for computer webcam. feed of images would be used
  // for the actual image processing for the dealer)
  navigator.mediaDevices.getUserMedia({ video: true, audio: false })
      .then(stream => {
          video.srcObject = stream;
          video.play();
          streaming = true;

          video.onloadedmetadata = () => {
              startProcessing(video, canvas);
          };
      })
      .catch(err => {
          console.error('Camera error:', err);
      });
};


// start processing video feed
function startProcessing(video, canvas) {
  const src = new cv.Mat(video.height, video.width, cv.CV_8UC4);
  const gray = new cv.Mat(video.height, video.width, cv.CV_8UC1);
  const binary = new cv.Mat(video.height, video.width, cv.CV_8UC1);
  const cap = new cv.VideoCapture(video);

  document.getElementById('captureButton').addEventListener('click', () => {
    cap.read(src);
    processFrame(src, gray, binary, canvas);
  });

  function processFrame() {
      // Grayscale Image
      cv.cvtColor(src, gray, cv.COLOR_RGBA2GRAY);

      // Gaussian Blur to reduce noise
      cv.GaussianBlur(gray, gray, new cv.Size(3,3), 0);

      // Threshold Image (Binary) with invert (numbers are black need to be white)
      cv.threshold(gray, binary, 0, 255, cv.THRESH_BINARY_INV + cv.THRESH_OTSU);

      // Finding Contours
      let contours = new cv.MatVector();
      let hierarchy = new cv.Mat();
      cv.findContours(binary, contours, hierarchy, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE);

      // Draw Contours
      let contourOutput = new cv.Mat();
      cv.cvtColor(binary, contourOutput, cv.COLOR_GRAY2RGB); // This is correct for drawing in color

      for (let i = 0; i < contours.size(); ++i) {
        let cnt = contours.get(i);
        let rect = cv.boundingRect(cnt);
      
        if (
          rect.width <= 0 || rect.height <= 0 ||
          rect.x < 0 || rect.y < 0 ||
          rect.x + rect.width > binary.cols ||
          rect.y + rect.height > binary.rows
        ) {
          cnt.delete();
          continue;
        }
      
        let symbolROI = binary.roi(rect);
        let size = symbolROI.size();
      
        if (size.width > 0 && size.height > 0) {
          let boxArea = rect.width * rect.height;
          let boxHeight = rect.height;
      
          if (boxArea > 500 && boxArea < 1100 && boxHeight > 5 && boxHeight < 35) {
            cv.drawContours(contourOutput, contours, i, new cv.Scalar(255, 0, 0), 1);
            cv.imshow('suitDisplay', symbolROI);
            console.log("suitHeight is " + boxHeight);
          } else if (boxArea > 700 && boxArea < 1100 && boxHeight >= 35 && boxHeight < 45) {
            cv.drawContours(contourOutput, contours, i, new cv.Scalar(0, 255, 0), 1);
            cv.imshow('rankDisplay', symbolROI);
            console.log("rankHeight is " + boxHeight);
          }
        }
          symbolROI.delete();
          cnt.delete();
        }
          cv.imshow(canvas, contourOutput);

          // delete all leftover data
          contourOutput.delete();
          contours.delete();
          hierarchy.delete();
      }
    }
    