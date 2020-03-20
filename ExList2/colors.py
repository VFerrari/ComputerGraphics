'''
MC937 - Computer Graphics
Exercise List 2 - Exercise 1: Color Converter

Simple color converter from one color space to another. 
Supports RGB (sRGB), CMY, HSV, XYZ and CIELAB.
Uses Illuminant d50 as white reference.

Author:
    Victor Ferreira Ferrari - RA 187890

Last modified: 19/03/2020
'''

import argparse
import numpy as np

def main():
    SPACES = np.array(['rgb', 'cmy', 'hsv', 'xyz', 'cielab'])
    
    # Arguments
    parser = argparse.ArgumentParser(description='Converts values on a color space to another.')
    parser.add_argument('orig' , choices=SPACES, metavar='original', help='Name of the original color space.')
    parser.add_argument('final', choices=SPACES, metavar='final', help='Name of the desired color space.')
    parser.add_argument('values', nargs=3, type=float, help='Values of color in the original space.Ranges:RGB <= 255, CMY <= 100.0, HSV Hue < 360.0, HSV Sat/Val <= 1.0')
    args = parser.parse_args()
    
    values = np.array(args.values)
    
    # Check if RGB has integer values
    if args.orig == 'rgb' and np.all(np.mod(values, 1) == 0) == False:
        raise TypeError('The values given for the RGB space are not integers!')
    
    # Check Ranges
    error = (values < 0).any() if args.orig != 'xyz' and args.orig != 'cielab' else 0
    if args.orig == 'hsv':
        error = error or values[0] >= 360.0 or values[1] > 1 or values[2] > 1
    elif args.orig == 'rgb':
        error = error or (values > 255).any()
    elif args.orig == 'cmy':
        error = error or (values > 100.0).any()
    
    if error:
        print("Out of range! See 'help' for information on ranges.")
        return False
    
    # Conversion
    values = color_converter(args.orig, args.final, values)
    
    msg = args.orig.upper() + ' to ' + args.final.upper() + f' {values[0]} {values[1]} {values[2]}'
    print(msg)

# Main conversion function.
# Converts any supported space to any other supported space.
def color_converter(s1, s2, values):
    
    # If the spaces are the same.
    if s1 == s2:
        return values
    
    # From XYZ to CIELAB.
    if s1 == 'xyz' and s2 == 'cielab':
        return xyz_to_cie(values)
    
    # From CIELAB to XYZ.
    if s1 == 'cielab' and s2 == 'xyz':
        return cie_to_xyz(values)
    
    # Any space to RGB.
    if s1 != 'rgb':
        if s1 == 'cmy':
            values /= 100
            values = rgb_and_cmy(values)
        elif s1 == 'xyz':
            values = xyz_to_rgb(values)
        elif s1 == 'hsv':
            values = hsv_rgb(values)
        else:
            values = cie_to_xyz(values)
            values = xyz_to_rgb(values)
    else:
        values /= 255
    
    # From RGB to final space.
    if s2 == 'cmy':
        values = rgb_and_cmy(values)
        values *= 100
    elif s2 == 'xyz':
        values = rgb_to_xyz(values)
    elif s2 == 'hsv':
        values = rgb_hsv(values)
    elif s2 == 'cielab':
        values = rgb_to_xyz(values)
        values = xyz_to_cie(values)
    else:
        values *= 255
        values = np.around(values).astype('int32')

    return values

# Conversion from sRGB to CMY and CMY to sRGB.
def rgb_and_cmy(values):
    return np.ones(3) - values

# Conversion from XYZ to sRGB.
# Using method from brucelindbloom.com
def xyz_to_rgb(xyz):

    # Linear Conversion
    # Because of sRGB, the matrix is adapted for chromatic change to D50 (Bradford).
    trf=np.array([[3.1338561, -1.6168667, -0.4906146],[-0.9787684, 1.9161415, 0.0334540],[0.0719453, -0.2289914, 1.4052427]])
    rgb = np.dot(trf, xyz)
    
    # sRGB Companding.
    # Boolean arrays: cases.
    case1 = rgb <= 0.0031308
    case2 = np.invert(case1)
    
    # Applying companding
    rgb[case1] *= 12.92
    rgb[case2] = 1.055*(rgb[case2]**(1/2.4)) - 0.055
    
    return rgb

# Conversion from HSV to sRGB.
def hsv_rgb(hsv):
    
    # Aux
    C = hsv[2] * hsv[1]
    H = hsv[0]/60
    
    # Intermediate Value X
    X = C * (1-abs(H % 2 - 1))
    
    # Conversion
    if 0 <= H <= 1:
        R,G,B = C,X,0
    elif H <= 2:
        R,G,B = X,C,0
    elif H <= 3:
        R,G,B = 0,C,X
    elif H <= 4:
        R,G,B = 0,X,C
    elif H <= 5:
        R,G,B = X,0,C
    elif H <= 6:
        R,G,B = C,0,X
    else:
        R,G,B = 0,0,0
    
    # Matching value
    m = hsv[2] - C
    return np.array([R+m, G+m, B+m])

# Conversion from CIELAB to XYZ.
# Made using method from Bruce Lindbloom's website: brucelindbloom.com.
def cie_to_xyz(cie):
    
    # CIE standard constants and reference values: Illuminant d50.
    X,Y,Z = 0.9642, 1.0, 0.8251
    k = 903.3
    eps = 0.008856
    
    # Intermediate Values
    fY = (cie[0] + 16)/116
    fX = fY + cie[1]/500
    fZ = fY - cie[2]/200
    
    # Calculating multiplicative terms for X.
    aux = fX**3
    x = (116*fX - 16)/k if aux <= eps else aux
    
    # Y is different
    y = cie[0]/k if cie[0] <= k*eps else fY**3
    
    # Z is the same as X.
    aux = fZ**3
    z = (116*aux - 16)/k if aux <= eps else aux
    
    return np.array([x*X,y*Y,z*Z])

# Conversion from sRGB to XYZ.
# Using method from brucelindbloom.com
def rgb_to_xyz(rgb):

    # Inverse sRGB Companding
    # Boolean Arrays: cases.
    case1 = rgb <= 0.04045
    case2 = np.invert(case1)
    
    # Applying companding.
    rgb[case1] /= 12.92
    rgb[case2] = ((rgb[case2] + 0.055)/1.055)**2.4

    # Linear conversion.
    trf=np.array([[0.4360747, 0.3850649, 0.1430804],[0.2225045, 0.7168786, 0.0606169],[0.0139322, 0.0971045, 0.7141733]])

    return np.dot(trf, rgb)

# Conversion from sRGB to HSV.
def rgb_hsv(rgb):
    
    # Value
    V = np.max(rgb)
    
    # Aux
    m = np.min(rgb)
    C = V - m
    
    # RGB
    R,G,B = rgb[0], rgb[1], rgb[2]
    
    # Hue
    if C == 0:
        hue = 0
    elif V == R:
        hue = 60*(G-B)/C
    elif V == G:
        hue = 60*(2+(B-R)/C)
    else:
        hue = 60*(4+(R-G)/C)
    
    # Saturation
    S = 0 if V==0 else C/V
    
    return np.array([hue,S,V])

# f for XYZ to CIELAB.
def f_xyz_cie(t):
    return ((24389/27)*t + 16)/116 if t <= 216/24389 else t**(1/3)

# Conversion from XYZ to CIELAB.
# Using method from brucelindbloom.com
def xyz_to_cie(xyz):
    
    # Reference values: Illuminant d50.
    Xn,Yn,Zn = 0.9642, 1.0, 0.8251

    aux = f_xyz_cie(xyz[1]/Yn)
    
    # f
    L = 116*aux - 16
    a = 500*(f_xyz_cie(xyz[0]/Xn) - aux)
    b = 200*(aux - f_xyz_cie(xyz[2]/Zn))
    
    return np.array([L,a,b])

if __name__ == '__main__':
    main()
