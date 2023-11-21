#include "text_renderer.h"

TextRender::TextRender() {
    if (!gltInit())
	{
		fprintf(stderr, "Failed to initialize glText\n");
	}
}

void TextRender::Render(Text &text, int horizontal_aligment, int vertical_aligment){
    gltBeginDraw();
    gltColor(text.color.r, text.color.g, text.color.b, text.color.a);
    gltDrawText2DAligned(
            text.text,
            text.pos.x,
            text.pos.y,
            text.scale,
            horizontal_aligment,
            vertical_aligment
    );
    gltEndDraw();
}

GLTtext *TextRender::Create(const char *string) {
    GLTtext *text = gltCreateText();

    gltSetText(text, string);

    return text;
}
