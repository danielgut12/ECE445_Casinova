// card_detector_core.js

function sharpenImage(src) {
    let blurred = new cv.Mat();
    let dst = new cv.Mat();
    cv.GaussianBlur(src, blurred, new cv.Size(0, 0), 3.5, 3.5);
    cv.addWeighted(src, 1.2, blurred, -0.5, 0, dst);
    blurred.delete();
    return dst;
  }
  
  function preprocessImage(src) {
    let gray = new cv.Mat();
    let blur = new cv.Mat();
    let thresh = new cv.Mat();
    cv.cvtColor(src, gray, cv.COLOR_RGBA2GRAY);
    cv.GaussianBlur(gray, blur, new cv.Size(5, 5), 0);
    cv.threshold(blur, thresh, 80, 255, cv.THRESH_BINARY_INV);
    gray.delete(); blur.delete();
    return thresh;
  }
  
  function findCardContours(thresh, frame) {
    let contours = new cv.MatVector();
    let hierarchy = new cv.Mat();
    cv.findContours(thresh, contours, hierarchy, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE);
  
    let boxes = [];
    for (let i = 0; i < contours.size(); ++i) {
      let cnt = contours.get(i);
      let rect = cv.boundingRect(cnt);
      let area = rect.width * rect.height;
      let aspect = rect.width / rect.height;
      if (area > 350 && area < 1500 && aspect > 0.2 && aspect < 2.0) {
        boxes.push({ contour: cnt, x: rect.x, y: rect.y });
      } else {
        cnt.delete();
      }
    }
  
    boxes.sort((a, b) => (a.y + a.x) - (b.y + b.x));
    hierarchy.delete();
    contours.delete();
  
    return boxes.length >= 2 ? [boxes[0].contour, boxes[1].contour] : [null, null];
  }
  
  function findBestMatch(input, templates, isSuit) {
    let bestMatch = "Unknown";
    let bestDiff = 10000;
    let templateSet = isSuit ? templates.Suits : templates.Ranks;
  
    for (let name in templateSet) {
      let tpl = templateSet[name];
      let resized = new cv.Mat();
      cv.resize(input, resized, tpl.size());
  
      let diff = new cv.Mat();
      cv.absdiff(resized, tpl, diff);
      let score = cv.countNonZero(diff);
  
      if (score < bestDiff) {
        bestDiff = score;
        bestMatch = name;
      }
  
      resized.delete();
      diff.delete();
    }
  
    return bestDiff < 2250 ? bestMatch : "Unknown";
  }
  
  function resizeCardImage(img, isSuit) {
    let dst = new cv.Mat();
    let size = isSuit ? new cv.Size(70, 100) : new cv.Size(70, 125);
    cv.resize(img, dst, size);
    return dst;
  }
  
  function extractImageFromContour(contour, src) {
    let rect = cv.boundingRect(contour);
    return src.roi(rect);
  }
  