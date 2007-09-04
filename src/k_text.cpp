/*
 * ux_text.c - Unix interface, text functions
 *
 * This file is part of Frotz.
 *
 * Frotz is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Frotz is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include "frotz/frotz.h"
#include "k_frotz.h"
#include "kwestview.h"

static int current_style = 0;
static int current_zfont = TEXT_FONT;

/*
 * os_font_data
 *
 * Return true if the given font is available. The font can be
 *
 *    TEXT_FONT
 *    PICTURE_FONT
 *    GRAPHICS_FONT
 *    FIXED_WIDTH_FONT
 *
 * The font size should be stored in "height" and "width". If
 * the given font is unavailable then these values must _not_
 * be changed.
 *
 */

int os_font_data(int font, int *height, int *width)
{
  return global_kwestview->getFontData(font, height, width);
}



/*
 * os_set_colour
 *
 * Set the foreground and background colours which can be:
 *
 *     DEFAULT_COLOUR
 *     BLACK_COLOUR
 *     RED_COLOUR
 *     GREEN_COLOUR
 *     YELLOW_COLOUR
 *     BLUE_COLOUR
 *     MAGENTA_COLOUR
 *     CYAN_COLOUR
 *     WHITE_COLOUR
 *
 *     MS-DOS 320 columns MCGA mode only:
 *
 *     GREY_COLOUR
 *
 *     Amiga only:
 *
 *     LIGHTGREY_COLOUR
 *     MEDIUMGREY_COLOUR
 *     DARKGREY_COLOUR
 *
 * There may be more colours in the range from 16 to 255; see the
 * remarks on os_peek_colour.
 *
 */

void os_set_colour(int new_foreground, int new_background)
{
  KwestView* kv = global_kwestview;

  kv->setFgColor(new_foreground);
  kv->setBgColor(new_background);

  if (new_foreground == 1)
    kv->resetFgColor();
  if (new_background == 1)
    kv->resetBgColor();

  os_set_text_style(current_style);
}



/*
 * os_set_text_style
 *
 * Set the current text style. Following flags can be set:
 *
 *     REVERSE_STYLE
 *     BOLDFACE_STYLE
 *     EMPHASIS_STYLE (aka underline aka italics)
 *     FIXED_WIDTH_STYLE
 *
 */

void os_set_text_style(int new_style)
{
  global_kwestview->selectFontAndStyle(current_zfont, new_style);
  current_style = new_style;
}



/*
 * os_set_font
 *
 * Set the font for text output. The interpreter takes care not to
 * choose fonts which aren't supported by the interface.
 *
 */

void os_set_font(int new_font)
{
  global_kwestview->selectFontAndStyle(new_font, current_style);
  current_zfont = new_font;
}



/*
 * os_display_char
 *
 * Display a character of the current font using the current colours and
 * text style. The cursor moves to the next position. Printable codes are
 * all ASCII values from 32 to 126, ISO Latin-1 characters from 160 to
 * 255, ZC_GAP (gap between two sentences) and ZC_INDENT (paragraph
 * indentation). The screen should not be scrolled after printing to the
 * bottom right corner.
 *
 */

void os_display_char(zchar c)
{
  if (c == ZC_GAP)
    return global_kwestview->writeString("  ");

  if (c == ZC_INDENT)
    return global_kwestview->writeString("   ");

  global_kwestview->writeString(QChar(c));
}



/*
 * os_display_string
 *
 * Pass a string of characters to os_display_char.
 *
 */

void os_display_string(const zchar *s)
{
  zchar c;
  QString qs;

  while ((c = (unsigned char) *s++) != 0)
    if (c == ZC_NEW_FONT || c == ZC_NEW_STYLE)
    {
      // First flush string buffer.
      if (qs.length())
      {
        global_kwestview->writeString(qs);
        qs = "";
      }

      // Set new font or style.
      int arg = (unsigned char) *s++;
      if (c == ZC_NEW_FONT)
        os_set_font(arg);
      if (c == ZC_NEW_STYLE)
        os_set_text_style(arg);
    }
    else
      qs += c;

  if (qs.length())
    global_kwestview->writeString(qs);
}



/*
 * os_char_width
 *
 * Return the width of the character in screen units.
 *
 */

int os_char_width(zchar c)
{
  return global_kwestview->charWidth(QChar(c));
}



/*
 * os_string_width
 *
 * Calculate the length of a word in screen units. Apart from letters,
 * the word may contain special codes:
 *
 *    NEW_STYLE - next character is a new text style
 *    NEW_FONT  - next character is a new font
 *
 */

int os_string_width(const zchar *s)
{
  int width = 0;
  zchar c;

  while ((c = *s++) != 0)
    if (c == ZC_NEW_STYLE || c == ZC_NEW_FONT)
      s++;
    else
      width += os_char_width(c);

  return width;
}



/*
 * os_set_cursor
 *
 * Place the text cursor at the given coordinates. Top left is (1,1).
 * y relates to rows, x to colsumns.
 *
 */

void os_set_cursor(int y, int x)
{
  global_kwestview->setXY(x-1, y-1);
}



/*
 * os_more_prompt
 *
 * Display a MORE prompt, wait for a keypress and remove the MORE
 * prompt from the screen.
 *
 */

void os_more_prompt()
{
  global_kwestview->morePrompt();
}
