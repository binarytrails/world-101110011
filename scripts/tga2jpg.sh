#! @file
#! @author Vsevolod (Seva) Ivanov
#! @brief convert is part of imagemagick
#!
for f in $(ls *tga); do
    convert $f "$(basename $f .tga).jpg";
done
