#pragma once

namespace AppStyle
{
inline constexpr char* stylesheet {"QPushButton, QToolButton"
                                   "{"
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
                                   "   background - color : #2077bc;"
                                   "   border-top-right-radius : 10px;"
                                   "   border-bottom-right-radius : 10px;"
                                   "   width : 15px;"
                                   "}"

};

inline constexpr char* blue {"#2077bc"};
inline constexpr char* softGrey {"#E0E0E0"};
} // namespace AppStyle
