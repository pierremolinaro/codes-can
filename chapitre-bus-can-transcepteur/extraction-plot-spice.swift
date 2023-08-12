#! /usr/bin/swift

import Foundation

let filePath = CommandLine.arguments [1]
let xFactor = Double (CommandLine.arguments [2])!

let contents = try! String (contentsOfFile: filePath)
let lines = contents.components (separatedBy: ["\n"])
//print ("\(lines.count) lines")
var points = [(Double, Double)] ()
var idx = 0
for aLine in lines {
  let scanner = Scanner (string: aLine)
  if let readIndex = scanner.scanInt (), readIndex == idx,
     let readTime = scanner.scanDouble (),
     let readY = scanner.scanDouble () {
    idx += 1
    points.append ((readTime * xFactor, readY))       
  }
}
//print ("\(points.count) points")
let (x0, y0) = points.remove (at: 0)
var s = "\\draw[very thick,draw=\\couleurCourbeSpice] (\(x0), \(y0))\n"
for (x, y) in points {
  s += "-- (\(x), \(y))\n"
}
s += ";\n"
try! s.write (toFile: filePath + ".txt", atomically: true, encoding: .utf8)
