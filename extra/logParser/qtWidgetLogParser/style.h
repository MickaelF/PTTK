#pragma once

namespace AppStyle
{
inline constexpr char* stylesheet {"QMenuBar"
                                   "{"
                                   "   background-color: white;"
                                   "}"
                                   "QScrollBar:vertical {            "
                                   "	border: 0px solid #999999;    "
                                   "	background:#E0E0E0;             "
                                   "	width:15px;                   "
                                   "	margin: 25px 0px 25px 0px;      "
                                   "}                                "
                                   "QScrollBar::handle:vertical {    "
                                   "    border-radius: 7px;"
                                   "	min-height: 40px;              "
                                   "	background-color: #2077bc;      "
                                   "   border : 3px solid #E0E0E0;"
                                   "}                                "
                                   "QScrollBar::add-line:vertical {  "
                                   "	height: 25px;                  "
                                   "	subcontrol-position: bottom;  "
                                   "	subcontrol-origin: margin;    "
                                   "}                                "
                                   "QScrollBar::sub-line:vertical {  "
                                   "	height: 25px;                 "
                                   "	subcontrol-position: top;     "
                                   "	subcontrol-origin: margin;    "
                                   "}                                "
                                   "QPushButton, QToolButton"
                                   "{"
                                   "   font-size: 12px;"
                                   "   border: 1px solid #2077bc;"
                                   "   border-radius: 15px;"
                                   "   min-height: 30px;"
                                   "   max-height: 30px;"
                                   "   min-width: 100px;"
                                   "   max-width: 100px;"
                                   "}"
                                   "QPushButton:default"
                                   "{"
                                   "   background-color: #2077bc;"
                                   "   color: #FFFFFF; "
                                   "}"
                                   "QToolButton::menu-button"
                                   "{"
                                   "   background-color : #2077bc;"
                                   "   border-top-right-radius : 10px;"
                                   "   border-bottom-right-radius : 10px;"
                                   "   width : 15px;"
                                   "}"
                                   "QToolButton#g_displayedFiles"
                                   "{"
                                   "   border : 0px;"
                                   "   font-style: underline;"
                                   "   color: #2077bc"
                                   "}"
                                   "QToolButton::menu-indicator"
                                   "{"
                                   "   image: none;"
                                   "}"

};

inline constexpr char* blue {"#2077bc"};
inline constexpr char* softGrey {"#E0E0E0"};
} // namespace AppStyle
