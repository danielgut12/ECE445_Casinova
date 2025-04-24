function onOpenCvReady() {
cv['onRuntimeInitialized'] = () => {
  console.log('OpenCV is ready');

  if (typeof cv !== 'object' || typeof cv.Size !== 'function') {
    console.error("cv not ready");
    return;
  }

  console.log("OpenCV is fully loaded.");

  // DOM references
  const canvas = document.getElementById('canvasOutput');
  const suit = document.getElementById('suitDisplay');
  const rank = document.getElementById('rankDisplay');
  const imgElement = document.getElementById('imageInput');

  // Templates
  const rankTemplates = {};
  const suitTemplates = {};
  const rankLabels = ['A','2','3','4','5','6','7','8','9','T','J','Q','K'];
  const suitLabels = ['H','D','S','C'];
  let templateReady = false;

  function loadTemplates(labels, folder, size, map, callback) {
    let loaded = 0;
    labels.forEach(label => {
      const img = new Image();
      img.src = `${folder}/${label}.jpg`;
      img.onload = () => {
        const mat = cv.imread(img);
        console.log('Loaded template:', label, mat); // ← This is what you add
        cv.cvtColor(mat, mat, cv.COLOR_RGBA2GRAY);
        cv.resize(mat, mat, size);
        map[label] = mat;
        loaded++;
        if (loaded === labels.length) callback();
      };
    });
  }

  // Load all templates, then run processing
  loadTemplates(rankLabels, 'templates/ranks', new cv.Size(70, 125), rankTemplates, () => {
    loadTemplates(suitLabels, 'templates/suits', new cv.Size(70, 100), suitTemplates, () => {
      console.log("All templates loaded.");
      templateReady = true;
    });
  });

  // Match function
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

  // Process button click
  document.getElementById('captureButton').addEventListener('click', () => {
    if (!templateReady) {

      console.log("Templates not ready yet.");
      return;
    }

    const src = cv.imread(imgElement);
    const gray = new cv.Mat();
    const binary = new cv.Mat();
    const contourOutput = new cv.Mat();

    let detectedSuit = null;
    let detectedRank = null;

    // Process image
    cv.cvtColor(src, gray, cv.COLOR_RGBA2GRAY);
    cv.GaussianBlur(gray, gray, new cv.Size(9, 9), 0);
    let otsuThresh = cv.threshold(gray, new cv.Mat(), 0, 255, cv.THRESH_BINARY_INV + cv.THRESH_OTSU);
    let tweakedThreshold = otsuThresh + 30
    cv.threshold(gray, binary, tweakedThreshold, 255, cv.THRESH_BINARY_INV);
    cv.cvtColor(binary, contourOutput, cv.COLOR_GRAY2RGB);

    let contours = new cv.MatVector();
    let hierarchy = new cv.Mat();
    cv.findContours(binary, contours, hierarchy, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE);

    for (let i = 0; i < contours.size(); ++i) {
      const cnt = contours.get(i);
      const rect = cv.boundingRect(cnt);

      if (rect.width <= 0 || rect.height <= 0 || rect.x < 0 || rect.y < 0 ||
          rect.x + rect.width > binary.cols || rect.y + rect.height > binary.rows) {
        cnt.delete();
        continue;
      }

      const symbolROI = binary.roi(rect);
      const boxArea = rect.width * rect.height;
      const boxHeight = rect.height;
      const aspectRatio = rect.width / rect.height;

      // Suit
      if (
        boxArea >= 0 && boxArea <= 2500 &&
        boxHeight >= 40 && boxHeight <= 50 &&
        aspectRatio >= 0.7 && aspectRatio <= 1.1
      ) {
        cv.drawContours(contourOutput, contours, i, new cv.Scalar(255, 0, 0), 1);
        const resized = new cv.Mat();
        cv.resize(symbolROI, resized, new cv.Size(70, 100));
        detectedSuit = matchTemplate(resized, suitTemplates);
        cv.imshow(suit, symbolROI);
        resized.delete();
      }
      // Rank
      else if (
        boxArea >= 0 && boxArea <= 4000 &&
        boxHeight >= 60 && boxHeight <= 75 &&
        aspectRatio >= 0.35 && aspectRatio <= 0.75
      ) {
        cv.drawContours(contourOutput, contours, i, new cv.Scalar(0, 255, 0), 1);
        const resized = new cv.Mat();
        cv.resize(symbolROI, resized, new cv.Size(70, 125));
        detectedRank = matchTemplate(resized, rankTemplates);
        cv.imshow(rank, symbolROI);
        resized.delete();
      }

      symbolROI.delete();
      cnt.delete();
    }

    // Show output
    cv.imshow(canvas, contourOutput);

    // Display label
    let cardText = 'Detected Card: —';
    if (detectedRank && detectedSuit) {
      const suitSymbols = { H: '♥', D: '♦', S: '♠', C: '♣' };
      const displaySuit = suitSymbols[detectedSuit] || detectedSuit;
      cardText = `Detected Card: ${detectedRank}${displaySuit}`;
    }
    document.getElementById('cardLabel').innerText = cardText;

    // Cleanup
    src.delete();
    gray.delete();
    binary.delete();
    contourOutput.delete();
    contours.delete();
    hierarchy.delete();
  });
};
}