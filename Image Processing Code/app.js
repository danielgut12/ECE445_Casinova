cv['onRuntimeInitialized'] = () => {
  console.log('OpenCV is ready');

  // flags
  let templateReady = false;
  let videoReady = false;

  // video / canvas vars
  const video = document.getElementById('videoInput');
  const canvas = document.getElementById('canvasOutput');
  const suit = document.getElementById('suitDisplay');
  const rank = document.getElementById('rankDisplay');
  let streaming = false;

  // Template Vars
  const rankTemplates = {};
  const suitTemplates = {};
  const rankLabels = ['A','2','3','4','5','6','7','8','9','T','J','Q','K'];
  const suitLabels = ['H','D','S','C'];

  function loadTemplates(labels, folder, size, map, callback) {
    let loaded = 0;
    labels.forEach(label => {
      const img = new Image();
      img.src = `${folder}/${label}.jpg`;
      img.onload = () => {
        const mat = cv.imread(img);
        cv.cvtColor(mat, mat, cv.COLOR_RGBA2GRAY);
        cv.resize(mat, mat, size);
        map[label] = mat;
        loaded++;
        if (loaded === labels.length) callback();
      };
    });
  }

  // Load Templates into consts
  loadTemplates(rankLabels, 'templates/ranks', new cv.Size(70, 125), rankTemplates, () => {
    loadTemplates(suitLabels, 'templates/suits', new cv.Size(70, 100), suitTemplates, () => {
      console.log("All templates loaded.");
      templateReady = true;
      checkAndStart();
    });
  });


  // snags the camera connected to device (just for computer webcam. feed of images would be used
  // for the actual image processing for the dealer)
  navigator.mediaDevices.getUserMedia({ video: true, audio: false })
      .then(stream => {
          video.srcObject = stream;
          video.play();
          streaming = true;

          video.onloadedmetadata = () => {
              videoReady = true;
              checkAndStart();
          };
      })
      .catch(err => {
          console.error('Camera error:', err);
      });

function checkAndStart() {
        if(templateReady && videoReady) startProcessing(video, canvas, suit, rank, rankTemplates, suitTemplates);
      }
};


// start processing video feed
function startProcessing(video, canvas, suit, rank, rankTemplates, suitTemplates) {
  const src = new cv.Mat(video.height, video.width, cv.CV_8UC4);
  const gray = new cv.Mat(video.height, video.width, cv.CV_8UC1);
  const binary = new cv.Mat(video.height, video.width, cv.CV_8UC1);
  const cap = new cv.VideoCapture(video);

  document.getElementById('captureButton').addEventListener('click', () => {
    cap.read(src);
    processFrame(src, gray, binary, canvas);
  });


  // input: symbolMat and the templateMap
  function matchTemplate(symbolMat, templateMap) {
    let bestLabel = null;
    let bestScore = Infinity;
  
    for (const label in templateMap) {
      const template = templateMap[label];
      const result = new cv.Mat();
      cv.matchTemplate(symbolMat, template, result, cv.TM_SQDIFF_NORMED);
      const { minVal } = cv.minMaxLoc(result);
      result.delete();
  
      if (minVal < bestScore) {
        bestScore = minVal;
        bestLabel = label;
      }
    }
  
    return bestLabel;
  }
  

  function processFrame() {

    // Suit and Rank Variables
    let detectedSuit = null;
    let detectedRank = null;


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
    
      // Delete if contour makes no sense
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
    
        // Suit
        if (boxArea > 500 && boxArea < 1100 && boxHeight > 5 && boxHeight < 35) {
          cv.drawContours(contourOutput, contours, i, new cv.Scalar(255, 0, 0), 1);
          let resized = new cv.Mat();
          cv.resize(symbolROI, resized, new cv.Size(70, 100));
          let label = matchTemplate(resized, suitTemplates);
          detectedSuit = label;
          console.log("Matched suit:", label);
          cv.imshow(suit, symbolROI);
          resized.delete();

        // Rank
        } else if (boxArea > 500 && boxArea < 1100 && boxHeight >= 35 && boxHeight < 45) {
          cv.drawContours(contourOutput, contours, i, new cv.Scalar(0, 255, 0), 1);
          let resized = new cv.Mat();
          cv.resize(symbolROI, resized, new cv.Size(70, 126));
          let label = matchTemplate(resized, rankTemplates);
          detectedRank = label;
          console.log("Matched rank:", label);
          cv.imshow(rank, symbolROI);
          resized.delete();
        }
      }
        symbolROI.delete();
        cnt.delete();
      }
      cv.imshow(canvas, contourOutput);

      // display detected suit and rank on html
      let cardText = 'Detected Card: —';
      if (detectedRank && detectedSuit) {
        const suitSymbols = { H: '♥', D: '♦', S: '♠', C: '♣' };
        const displaySuit = suitSymbols[detectedSuit] || detectedSuit;
        cardText = `Detected Card: ${detectedRank}${displaySuit}`;
      }
      document.getElementById('cardLabel').innerText = cardText;      
    
      // delete all leftover data
      contourOutput.delete();
      contours.delete();
      hierarchy.delete();
    }
  }
    