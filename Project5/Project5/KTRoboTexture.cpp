#include "KTRoboTexture.h"
#include "memory.h"
#include "KTRoboMesh.h"
#include "KTRoboDebugText.h"

using namespace KTROBO;

int Texture::getTexture(char* tex_name, int index_count) {
	
	// すでにロードされていた場合はロードは行われない
	CS::instance()->enter(CS_RENDERDATA_CS, "gettex");
	if (this->texturepart_index.find(tex_name) != texturepart_index.end()) {
		CS::instance()->leave(CS_RENDERDATA_CS, "gettex");
		return texturepart_index[tex_name];
	}

	
	// ないので作る
	TexturePart* p = new TexturePart(index_count);
	try {
	p->loadClass(this->loader, tex_name);
	}catch(GameError* err) {
		delete p;
		CS::instance()->leave(CS_RENDERDATA_CS, "gettex");
		throw err;
	}
	int size = this->parts.size();


	
	parts.push_back(p); // まだpはindexbufferが作られていないことに注意
	texturepart_index.insert(pair<string,int>(string(tex_name), size));
	
	CS::instance()->leave(CS_RENDERDATA_CS, "gettex");
	return size;
}





int Texture::getRenderTex(int tex_index, unsigned int color, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height) {
	// 空いている場所があるかどうか調べる
	CS::instance()->enter(CS_RENDERDATA_CS, "rendertex");
	int size = unuse_render_texs.size();
	RenderTex* use_tex;
	if (tex_index < this->parts.size()) {
		// 該当のテクスチャのis_need_loadをtrueにする
		// loadtovertextextureではis_need_loadの前にis_useを見るようにすること

		parts[tex_index]->setIsNeedLoad(true);
	} else {
		// おかしい
		CS::instance()->leave(CS_RENDERDATA_CS, "rendertex");
		throw new GameError(KTROBO::WARNING, "there is no tex in getRenderTex");
	}
	if (size >0) {
		use_tex  =*unuse_render_texs.begin();
		unuse_render_texs.erase(use_tex);
	} else {
		// 空いているIDがないかどうか調べる
		int s = unuse_render_tex_ids.size();
		use_tex = new RenderTex();
		if (s > 0) {
			int id = *unuse_render_tex_ids.begin();
			unuse_render_tex_ids.erase(id);
			use_tex->id = id;
			int rentexindex = render_texs.size();
			render_tex_indexs.insert(pair<int,int>(id,rentexindex));
		} else {
			// 今のrender_texのサイズがIDとなるはず
			int id = render_texs.size();
			use_tex->id = id;
			render_tex_indexs.insert(pair<int,int>(id,id));
		}
		render_texs.push_back(use_tex);
	}	
	use_tex->color = color;
	use_tex->height = height;
	// idはそのまま
	use_tex->is_need_load = true;
	use_tex->is_render = false;
	use_tex->is_use = true;
	use_tex->tex_height = tex_height;
	use_tex->tex_index = tex_index;
	use_tex->tex_width = tex_width;
	use_tex->tex_x = tex_x;
	use_tex->tex_y = tex_y;
	use_tex->width = width;
	use_tex->x = x;
	use_tex->y = y;
	int ans = use_tex->id;
	parts[tex_index]->setRenderTexId(use_tex->id);
	CS::instance()->leave(CS_RENDERDATA_CS, "rendertex");
	return ans;
}
int Texture::getRenderBillBoard(int tex_index, unsigned int color, YARITORI MYMATRIX* world, float width, float height, int tex_x, int tex_y, int tex_width, int tex_height) {

	// 空いている場所があるかどうか調べる
	CS::instance()->enter(CS_RENDERDATA_CS, "renderbillb");
	int size = unuse_render_texs.size();
	RenderBillBoard* use_bill;
	if (tex_index < this->parts.size()) {
		// 該当のテクスチャのis_need_loadをtrueにする
		// loadtovertextextureではis_need_loadの前にis_useを見るようにすること

		parts[tex_index]->setIsNeedLoad(true);
	} else {
		// おかしい
		CS::instance()->leave(CS_RENDERDATA_CS, "renderbillb");
		throw new GameError(KTROBO::WARNING, "there is no tex in getRenderBillBoard");
	}
	if (size >0) {
		// unuse_bill_boardsのなかのものを利用する場合はIDも再利用する
		// indexもそのまま is_useだけfalseになってる感じ
		use_bill  =*unuse_bill_boards.begin();
		unuse_bill_boards.erase(use_bill);
	} else {
		// 空いているIDがないかどうか調べる
		int s = unuse_bill_board_ids.size();
		use_bill = new RenderBillBoard();
		if (s > 0) {

			int id = *unuse_bill_board_ids.begin();
			unuse_bill_board_ids.erase(id);
			use_bill->id = id;
			int bill_index = bill_boards.size();
			bill_board_indexs.insert(pair<int,int>(id, bill_index));
		} else {
			// 今のrender_texのサイズがIDとなるはず
			int id = bill_boards.size();
			use_bill->id = id;
			bill_board_indexs.insert(pair<int,int>(id,id));

		}
		bill_boards.push_back(use_bill);
	}	
	use_bill->color = color;
	use_bill->height = height;
	// idはそのまま
	use_bill->is_need_load = true;
	use_bill->is_render = false;
	use_bill->is_use = true;
	use_bill->tex_height = tex_height;
	use_bill->tex_index = tex_index;
	use_bill->tex_width = tex_width;
	use_bill->tex_x = tex_x;
	use_bill->tex_y = tex_y;
	use_bill->width = width;
	use_bill->world = *world;
	
	int ans = use_bill->id;
	parts[tex_index]->setBillBoardId(use_bill->id);
	CS::instance()->leave(CS_RENDERDATA_CS, "renderbillb");
	return ans;
}

void Texture::setRenderTexParam(int render_tex_index, unsigned int color, int x, int y, int width, int height, int tex_x, int tex_y, int tex_width, int tex_height) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setrendertexparam");
	if (render_tex_indexs.find(render_tex_index) != render_tex_indexs.end()) {
		RenderTex* e = render_texs[render_tex_indexs[render_tex_index]];
		e->color = color;
		e->x = x;
		e->y = y;
		e->height = height;
		e->is_need_load = true;
		e->tex_height = tex_height;
		e->tex_width = tex_width;
		e->tex_x = tex_x;
		e->tex_y = tex_y;
		e->width = width;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setrendertexparam");
}
void Texture::setRenderBillBoardParam(int bill_index, unsigned int color, YARITORI MYMATRIX* world, float width, float height, int tex_x, int tex_y, int tex_width, int tex_height) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setbillboardparam");
	if (bill_board_indexs.find(bill_index) != bill_board_indexs.end()) {
		RenderBillBoard* e = bill_boards[bill_board_indexs[bill_index]];
		e->color = color;
		e->world = *world;
		e->height = height;
		e->is_need_load = true;
		e->tex_height = tex_height;
		e->tex_width = tex_width;
		e->tex_x = tex_x;
		e->tex_y = tex_y;
		e->width = width;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setbillboardparam");

}

void Texture::setRenderTexColor(int render_tex_index, unsigned int color) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setrendertexcolor");
	if (render_tex_indexs.find(render_tex_index) != render_tex_indexs.end()) {
		RenderTex* e = render_texs[render_tex_indexs[render_tex_index]];
		e->color = color;
		e->is_need_load = true;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setrendertexcolor");
}
void Texture::setRenderBillBoardColor(int bill_index, unsigned int color) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setbillboardcolor");
	if (bill_board_indexs.find(bill_index) != bill_board_indexs.end()) {
		RenderBillBoard* e = bill_boards[bill_board_indexs[bill_index]];
		e->color = color;	
		e->is_need_load = true;	
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setbillboardcolor");

}

void Texture::setRenderTexPos(int render_tex_index, int x, int y) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setrendertexpos");
	if (render_tex_indexs.find(render_tex_index) != render_tex_indexs.end()) {
		RenderTex* e = render_texs[render_tex_indexs[render_tex_index]];
		e->x = x;
		e->y = y;
		e->is_need_load = true;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setrendertexpos");


}
void Texture::setRenderBillBoardPos(int bill_index, YARITORI MYMATRIX* world) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setbillboardpos");
	if (bill_board_indexs.find(bill_index) != bill_board_indexs.end()) {
		RenderBillBoard* e = bill_boards[bill_board_indexs[bill_index]];
		e->world = *world;	
		e->is_need_load = true;	
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setbillboardpos");


}

void Texture::setRenderTexWH(int render_tex_index, int width, int height) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setrendertexwh");
	if (render_tex_indexs.find(render_tex_index) != render_tex_indexs.end()) {
		RenderTex* e = render_texs[render_tex_indexs[render_tex_index]];
		e->width = width;
		e->height = height;
		e->is_need_load = true;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setrendertexwh");



}


void Texture::setRenderBillBoardWH(int bill_index, float width, float height) {
	
	CS::instance()->enter(CS_RENDERDATA_CS, "setbillboardwh");
	if (bill_board_indexs.find(bill_index) != bill_board_indexs.end()) {
		RenderBillBoard* e = bill_boards[bill_board_indexs[bill_index]];
		e->width = width;
		e->height = height;
		e->is_need_load = true;	
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setbillboardwh");
}

void Texture::setRenderTexTexPos(int render_tex_index, int tex_x, int tex_y, int tex_width, int tex_height) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setrendertextpos");
	if (render_tex_indexs.find(render_tex_index) != render_tex_indexs.end()) {
		RenderTex* e = render_texs[render_tex_indexs[render_tex_index]];

		if ((e->tex_x != tex_x) || (e->tex_y != tex_y) || (e->tex_width != tex_width) || (e->tex_height != tex_height)) {
			e->tex_x = tex_x;
			e->tex_y = tex_y;
			e->tex_width = tex_width;
			e->tex_height = tex_height;
			e->is_need_load = true;
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setrendertextpos");
}



void Texture::setRenderBillBoardTexPos(int bill_index, int tex_x, int tex_y, int tex_width, int tex_height) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setbillboardtpos");
	if (bill_board_indexs.find(bill_index) != bill_board_indexs.end()) {
		RenderBillBoard* e = bill_boards[bill_board_indexs[bill_index]];
		if ((e->tex_x != tex_x) || (e->tex_y != tex_y) || (e->tex_width != tex_width) || (e->tex_height != tex_height)) {
			e->tex_x = tex_x;
			e->tex_y = tex_y;
			e->tex_width = tex_width;
			e->tex_height = tex_height;
			e->is_need_load = true;	
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setbillboardtpos");

}

void Texture::setRenderTexIsRender(int render_tex_index, bool t) {

	CS::instance()->enter(CS_RENDERDATA_CS, "setrendertexisrender");
	if (render_tex_indexs.find(render_tex_index) != render_tex_indexs.end()) {
		RenderTex* e = render_texs[render_tex_indexs[render_tex_index]];
		TexturePart* p = parts[e->tex_index];
		e->is_render = t;
//		e->is_need_load = true;
		p->setIsNeedLoad(true);
	//	p->setIsIndexLoad(false);
		if (t) {
		p->setRenderTexId(e->id);
		} else {
			p->eraseRenderTexId(e->id);
		}

	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setrendertexisrender");
}


void Texture::setRenderBillBoardIsRender(int bill_index, bool t) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setbillboardisrender");
	if (bill_board_indexs.find(bill_index) != bill_board_indexs.end()) {
		RenderBillBoard* e = bill_boards[bill_board_indexs[bill_index]];
		TexturePart* p = parts[e->tex_index];
		e->is_render = t;
//		e->is_need_load = true;	
		p->setIsNeedLoad(true);
	//	p->setIsIndexLoad(false);
		if (t) {
		p->setBillBoardId(e->id);
		} else {
			p->eraseBillBoardId(e->id);
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setbillboardisrender");
}
	
void Texture::setRenderTexChangeTex(int render_tex_index, int tex_index, int new_tex_index) {
	// 描画されるテクスチャを変えるには・・・
	// render_texのis_need_loadは変えなくてよい（tex_indexは頂点テクスチャの中にないので)
	// 該当するテクスチャのis_need_loadをtrueにしなくてはならない
	
	CS::instance()->enter(CS_RENDERDATA_CS, "setrendertexchangetex");
	if (render_tex_indexs.find(render_tex_index) != render_tex_indexs.end()) {
		RenderTex* t = render_texs[render_tex_indexs[render_tex_index]];
		TexturePart* p = parts[tex_index];
		TexturePart* p2 = parts[new_tex_index];
		t->tex_index = new_tex_index;
		p->setIsNeedLoad(true);
		p2->setIsNeedLoad(true);
		p->eraseRenderTexId(t->id);
		p2->setRenderTexId(t->id);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setrendertexchangetex");
}


void Texture::setRenderBillBoardChangeTex(int bill_index, int tex_index, int new_tex_index) {
	CS::instance()->enter(CS_RENDERDATA_CS, "setbillboardchangetex");
	if (bill_board_indexs.find(bill_index) != bill_board_indexs.end()) {
		RenderBillBoard* t = bill_boards[bill_board_indexs[bill_index]];
		TexturePart* p = parts[tex_index];
		TexturePart* p2 = parts[new_tex_index];
		t->tex_index = new_tex_index;
		p->setIsNeedLoad(true);
		p2->setIsNeedLoad(true);
		p->eraseBillBoardId(t->id);
		p2->setBillBoardId(t->id);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "setbillboardchangetex");
}


void Texture::deleteRenderTex(int render_tex_index) {
	
	// ロックをどうするか考えどころ RENDERDATAだけで大丈夫そう・・？
	CS::instance()->enter(CS_RENDERDATA_CS, "deleterendertex");
	if (render_tex_indexs.find(render_tex_index) != render_tex_indexs.end()) {
		RenderTex* t = render_texs[render_tex_indexs[render_tex_index]];
		TexturePart* p = parts[t->tex_index];
		// p のis_need_loadをtrueにするのはdelete処理を実際に行ってから
		erase_render_tex_ids.insert(t->id);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "deleterendertex");
}


void Texture::deleteRenderBillBoard(int bill_id) {
	// ロックをどうするか考えどころ
	CS::instance()->enter(CS_RENDERDATA_CS, "deletebillboard");
	if (bill_board_indexs.find(bill_id) != bill_board_indexs.end()) {
		RenderBillBoard* t = bill_boards[bill_board_indexs[bill_id]];
		erase_bill_board_ids.insert(t->id);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "deletebillboard");

}


void Texture::lightdeleteAllRenderTex() {
	CS::instance()->enter(CS_RENDERDATA_CS, "tes");
	vector<TexturePart*>::iterator it = parts.begin();
	set<int> erasedayo_ids;

	while (it != parts.end()) {
		TexturePart* pp = *it;
		set<int>::iterator itt = pp->render_tex_ids.begin();
		while (itt != pp->render_tex_ids.end()) {

			int ii = *itt;
			erasedayo_ids.insert(ii);

			itt++;
		}

		it++;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "tes");
	set<int>::iterator ite = erasedayo_ids.begin();
	CS::instance()->enter(CS_RENDERDATA_CS, "tes");
	while(ite != erasedayo_ids.end()) {
		int ii = *ite;
		lightdeleteRenderTex(ii);
		ite++;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "tes");

	erasedayo_ids.clear();

}


void Texture::lightdeleteAllBillBoard() {
	CS::instance()->enter(CS_RENDERDATA_CS, "tes");
	vector<TexturePart*>::iterator it = parts.begin();
	set<int> erasedayo_ids;

	while (it != parts.end()) {
		TexturePart* pp = *it;
		set<int>::iterator itt = pp->bill_board_ids.begin();
		while (itt != pp->bill_board_ids.end()) {

			int ii = *itt;
			erasedayo_ids.insert(ii);

			itt++;
		}

		it++;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "tes");
	set<int>::iterator ite = erasedayo_ids.begin();
	CS::instance()->enter(CS_RENDERDATA_CS, "tes");
	while(ite != erasedayo_ids.end()) {
		int ii = *ite;
		lightdeleteRenderBillBoard(ii);
		ite++;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "tes");

	erasedayo_ids.clear();




}

void Texture::lightdeleteRenderTex(int render_tex_index) {
	// ロックをどうするか考えどころ RENDERDATAだけで大丈夫そう・・？
	CS::instance()->enter(CS_RENDERDATA_CS, "lightdeleterendertex");
	if (render_tex_indexs.find(render_tex_index) != render_tex_indexs.end()) {
		RenderTex* t = render_texs[render_tex_indexs[render_tex_index]];
		TexturePart* p = parts[t->tex_index];
		// vectorから削除しないのでフラグを立てるだけ
		t->is_use = false;
		p->eraseRenderTexId(t->id);
		p->setIsNeedLoad(true);
		unuse_render_texs.insert(t);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "lightdeleterendertex");

}
void Texture::lightdeleteRenderBillBoard(int bill_id) {
	// ロックをどうするか考えどころ
	CS::instance()->enter(CS_RENDERDATA_CS, "deletebillboard");
	if (bill_board_indexs.find(bill_id) != bill_board_indexs.end()) {
		RenderBillBoard* t = bill_boards[bill_board_indexs[bill_id]];
		TexturePart* p = parts[t->tex_index];
		p->eraseBillBoardId(t->id);
		p->setIsNeedLoad(true);
		t->is_use = false;
		unuse_bill_boards.insert(t);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "deletebillboard");


}

void Textures::setDeleteAll() {
	if (texs.size()) {
		texs[0]->deleteAll();
		texs[1]->deleteAll();
	}
}


void Texture::deleteAll() {
	// 時間とか気にしないで全てdeleteする

	CS::instance()->enter(CS_RENDERDATA_CS, "deleteall");
	
	is_all_delete = true;

	CS::instance()->leave(CS_RENDERDATA_CS, "deleteall");

}

void Texture::_sendinfoToVertexTextureTex(Graphics* g, TEXTURE_VTEX_STRUCT_TEX* v, int size) {
	D3D11_MAPPED_SUBRESOURCE subresource;
	unsigned int stride = sizeof(TEXTURE_VTEX_STRUCT_TEX);
	unsigned int offset = 0;

	CS::instance()->enter(CS_DEVICECON_CS, "sendinfotex");
	// set
	g->getDeviceContext()->Map(Texture::vertextex_vertexbuffer_tex,0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	memcpy(subresource.pData, v, sizeof(TEXTURE_VTEX_STRUCT_TEX)*size);//KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE);
	g->getDeviceContext()->Unmap(Texture::vertextex_vertexbuffer_tex,0);

	D3D11_VIEWPORT vp;
	
	vp.Height = this->vtex_tex->height;
	vp.Width = this->vtex_tex->width;
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;


	float ccc[] = {
		0.2f,0.2f,0.8f,0.7f};

	g->getDeviceContext()->OMSetRenderTargets(1, &vtex_tex->target_view, mss_for_vertextex_tex.depthstencilview);
	g->getDeviceContext()->RSSetViewports(1, &vp);
	g->getDeviceContext()->ClearDepthStencilView(mss_for_vertextex_tex.depthstencilview,  D3D11_CLEAR_DEPTH/* | D3D11_CLEAR_STENCIL*/,1.0f, 0 );
	//g->getDeviceContext()->ClearRenderTargetView(combined_matrix_texture->target_view, ccc);

	g->getDeviceContext()->IASetInputLayout(mss_for_vertextex_tex.vertexlayout );
	ID3D11Buffer* tt[] = {vertextex_vertexbuffer_tex};
	unsigned int ttt[] = { sizeof(TEXTURE_VTEX_STRUCT_TEX)};
	unsigned int tttt[] = {0};
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, tt, ttt, tttt );

	//stride = sizeof(COMBINEDMATRIXCALC_CBUF);

	//g->getDeviceContext()->IASetVertexBuffers( 1, 1, &combined_matrix_watasu_offsetbuffer, &stride, &offset );
	g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	g->getDeviceContext()->IASetIndexBuffer(vertextex_indexbuffer, DXGI_FORMAT_R16_UINT,0);
	g->getDeviceContext()->RSSetState(mss_for_vertextex_tex.rasterstate);

	float blendFactor[4] = {1.0f,1.0f,1.0f,1.0f};

	g->getDeviceContext()->OMSetBlendState(mss_for_vertextex_tex.blendstate, blendFactor,0xFFFFFFFF);
	g->getDeviceContext()->VSSetShader(mss_for_vertextex_tex.vs, NULL, 0);
	g->getDeviceContext()->GSSetShader(mss_for_vertextex_tex.gs, NULL, 0);	
	g->getDeviceContext()->PSSetShader(mss_for_vertextex_tex.ps, NULL, 0);
//	g->getDeviceContext()->VSSetShaderResources(0,1,&->view);
//	g->getDeviceContext()->VSSetShaderResources(1,1,&matrix_local_texture->view);
	g->getDeviceContext()->DrawIndexed(size*3, 0,0);
	
//	g->getDeviceContext()->Flush();
//	g->getDeviceContext()->PSSetShaderResources(0,1,&testtt);
//	g->getDeviceContext()->GSSetShader(NULL, NULL,0);
	
	ID3D11RenderTargetView* t = g->getRenderTargetView();
	g->getDeviceContext()->OMSetRenderTargets(1, &t, NULL);
	g->getDeviceContext()->RSSetViewports(1, g->getViewPort());
//	g->getDeviceContext()->PSSetShaderResources(0,1,&testtt);
	tt[0] = NULL;
//	tt[1] = NULL;
	ttt[0] =0;
//	ttt[1] = 0;
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, tt, ttt, tttt );
	// unset



	CS::instance()->leave(CS_DEVICECON_CS, "sendinfotex");
}


void Texture::_sendinfoToVertexTextureBill(Graphics* g, TEXTURE_VTEX_STRUCT_BILL* b, int size) {
	CS::instance()->enter(CS_DEVICECON_CS, "sendinfobill");

	D3D11_MAPPED_SUBRESOURCE subresource;
	unsigned int stride = sizeof(TEXTURE_VTEX_STRUCT_BILL);
	unsigned int offset = 0;

	// set
	g->getDeviceContext()->Map(Texture::vertextex_vertexbuffer_bill,0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	memcpy(subresource.pData, b, sizeof(TEXTURE_VTEX_STRUCT_BILL)*size);//KTROBO_MESH_INSTANCED_ANIMELOADSTRUCT_TEMPSIZE);
	g->getDeviceContext()->Unmap(Texture::vertextex_vertexbuffer_bill,0);

	D3D11_VIEWPORT vp;
	
	vp.Height = this->vtex_bill->height;
	vp.Width = this->vtex_bill->width;
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;


	float ccc[] = {
		0.2f,0.2f,0.8f,0.7f};

	g->getDeviceContext()->OMSetRenderTargets(1, &vtex_bill->target_view, mss_for_vertextex_bill.depthstencilview);
	g->getDeviceContext()->RSSetViewports(1, &vp);
	g->getDeviceContext()->ClearDepthStencilView(mss_for_vertextex_bill.depthstencilview,  D3D11_CLEAR_DEPTH/* | D3D11_CLEAR_STENCIL*/,1.0f, 0 );
	//g->getDeviceContext()->ClearRenderTargetView(combined_matrix_texture->target_view, ccc);

	g->getDeviceContext()->IASetInputLayout(mss_for_vertextex_bill.vertexlayout );
	ID3D11Buffer* tt[] = {vertextex_vertexbuffer_bill};
	unsigned int ttt[] = { sizeof(TEXTURE_VTEX_STRUCT_BILL)};
	unsigned int tttt[] = {0};
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, tt, ttt, tttt );

	//stride = sizeof(COMBINEDMATRIXCALC_CBUF);

	//g->getDeviceContext()->IASetVertexBuffers( 1, 1, &combined_matrix_watasu_offsetbuffer, &stride, &offset );
	g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	g->getDeviceContext()->IASetIndexBuffer(vertextex_indexbuffer, DXGI_FORMAT_R16_UINT,0);
	g->getDeviceContext()->RSSetState(mss_for_vertextex_bill.rasterstate);

	float blendFactor[4] = {1.0f,1.0f,1.0f,1.0f};

	g->getDeviceContext()->OMSetBlendState(mss_for_vertextex_bill.blendstate, blendFactor,0xFFFFFFFF);
	g->getDeviceContext()->VSSetShader(mss_for_vertextex_bill.vs, NULL, 0);
	g->getDeviceContext()->GSSetShader(mss_for_vertextex_bill.gs, NULL, 0);	
	g->getDeviceContext()->PSSetShader(mss_for_vertextex_bill.ps, NULL, 0);
//	g->getDeviceContext()->VSSetShaderResources(0,1,&->view);
//	g->getDeviceContext()->VSSetShaderResources(1,1,&matrix_local_texture->view);
	g->getDeviceContext()->DrawIndexed(size*3, 0,0);
	
//	g->getDeviceContext()->Flush();
//	g->getDeviceContext()->PSSetShaderResources(0,1,&testtt);
//	g->getDeviceContext()->GSSetShader(NULL, NULL,0);
	
	ID3D11RenderTargetView* t = g->getRenderTargetView();
	g->getDeviceContext()->OMSetRenderTargets(1, &t, NULL);
	g->getDeviceContext()->RSSetViewports(1, g->getViewPort());
//	g->getDeviceContext()->PSSetShaderResources(0,1,&testtt);
	tt[0] = NULL;
//	tt[1] = NULL;
	ttt[0] =0;
//	ttt[1] = 0;
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, tt, ttt, tttt );
	// unset

	CS::instance()->leave(CS_DEVICECON_CS, "sendinfobill");
}

void Texture::_renderTex(Graphics* g, TexturePart* p) {


	D3D11_VIEWPORT vp;
	
	vp.Height = g->getScreenHeight();
	vp.Width = g->getScreenWidth();
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;


	float ccc[] = {
		0.2f,0.2f,0.8f,0.7f};
	ID3D11RenderTargetView* v = g->getRenderTargetView();
	this->updateCBuf1(g,p);
	g->getDeviceContext()->OMSetRenderTargets(1, &v, mss_for_render_tex.depthstencilview);
	g->getDeviceContext()->RSSetViewports(1, &vp);
	g->getDeviceContext()->ClearDepthStencilView(mss_for_render_tex.depthstencilview,  D3D11_CLEAR_DEPTH/* | D3D11_CLEAR_STENCIL*/,1.0f, 0 );
	//g->getDeviceContext()->ClearRenderTargetView(combined_matrix_texture->target_view, ccc);
	g->getDeviceContext()->VSSetConstantBuffers(0,1,&cbuf1_buffer);

	g->getDeviceContext()->VSSetShaderResources(0,1,&p->getClass()->view);
	g->getDeviceContext()->PSSetShaderResources(0,1,&p->getClass()->view);
	g->getDeviceContext()->VSSetShaderResources(1,1,&vtex_tex->view);
	g->getDeviceContext()->IASetInputLayout(mss_for_render_tex.vertexlayout );
	ID3D11Buffer* tt[] = {render_vertexbuffer};
	unsigned int ttt[] = { sizeof(TEXTURE_RENDER_STRUCT)};
	unsigned int tttt[] = {0};
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, tt, ttt, tttt );

	//stride = sizeof(COMBINEDMATRIXCALC_CBUF);

	//g->getDeviceContext()->IASetVertexBuffers( 1, 1, &combined_matrix_watasu_offsetbuffer, &stride, &offset );
	g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	g->getDeviceContext()->IASetIndexBuffer(p->indexbuffer_tex, DXGI_FORMAT_R16_UINT,0);
	g->getDeviceContext()->RSSetState(mss_for_render_tex.rasterstate);
	g->getDeviceContext()->VSSetSamplers(0,1,&Mesh::p_sampler);
	g->getDeviceContext()->PSSetSamplers(0,1,&DebugTexts::instance()->p_sampler);
	float blendFactor[4] = {1.0f,1.0f,1.0f,1.0f};

	g->getDeviceContext()->OMSetBlendState(mss_for_render_tex.blendstate, blendFactor,0xFFFFFFFF);
	g->getDeviceContext()->VSSetShader(mss_for_render_tex.vs, NULL, 0);
	g->getDeviceContext()->GSSetShader(mss_for_render_tex.gs, NULL, 0);	
	g->getDeviceContext()->PSSetShader(mss_for_render_tex.ps, NULL, 0);
	//	g->getDeviceContext()->VSSetShaderResources(0,1,&->view);
	//	g->getDeviceContext()->VSSetShaderResources(1,1,&matrix_local_texture->view);
	g->getDeviceContext()->DrawIndexed(p->index_count_tex, 0,0);

	//	g->getDeviceContext()->Flush();
	//	g->getDeviceContext()->PSSetShaderResources(0,1,&testtt);
	//	g->getDeviceContext()->GSSetShader(NULL, NULL,0);

	ID3D11RenderTargetView* t = g->getRenderTargetView();
	g->getDeviceContext()->OMSetRenderTargets(1, &t, NULL);
	g->getDeviceContext()->RSSetViewports(1, g->getViewPort());
	//	g->getDeviceContext()->PSSetShaderResources(0,1,&testtt);
		tt[0] = NULL;
	//	tt[1] = NULL;
		ttt[0] =0;
	//	ttt[1] = 0;
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, tt, ttt, tttt );
	// unset

}

void Texture::_renderBill(Graphics* g, TexturePart* p) {

	D3D11_VIEWPORT vp;
	
	vp.Height = g->getScreenHeight();
	vp.Width = g->getScreenWidth();
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;


	float ccc[] = {
		0.2f,0.2f,0.8f,0.7f};
	ID3D11RenderTargetView* v = g->getRenderTargetView();
	ID3D11RenderTargetView* ggt;
	ID3D11DepthStencilView* gggt;

	g->getDeviceContext()->OMSetRenderTargets(1, &v, Mesh::pDepthStencilView);
	g->getDeviceContext()->RSSetViewports(1, &vp);
//	g->getDeviceContext()->ClearDepthStencilView(mss_for_render_tex.depthstencilview,  D3D11_CLEAR_DEPTH/* | D3D11_CLEAR_STENCIL*/,1.0f, 0 );
	//g->getDeviceContext()->ClearRenderTargetView(combined_matrix_texture->target_view, ccc);
	ID3D11Buffer* test[] = 
	{ cbuf1_buffer,cbuf2_buffer};
	g->getDeviceContext()->VSSetConstantBuffers(0,2,test);
//	g->getDeviceContext()->VSSetConstantBuffers(1,1,&cbuf1_buffer);
	g->getDeviceContext()->VSSetShaderResources(0,1,&p->getClass()->view);
	g->getDeviceContext()->PSSetShaderResources(0,1,&p->getClass()->view);
	g->getDeviceContext()->VSSetShaderResources(1,1,&vtex_bill->view);
	g->getDeviceContext()->IASetInputLayout(mss_for_render_bill.vertexlayout );
	ID3D11Buffer* tt[] = {render_vertexbuffer};
	unsigned int ttt[] = { sizeof(TEXTURE_RENDER_STRUCT)};
	unsigned int tttt[] = {0};
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, tt, ttt, tttt );
	g->getDeviceContext()->VSSetSamplers(0,1,&Mesh::p_sampler);
	g->getDeviceContext()->PSSetSamplers(0,1,&DebugTexts::instance()->p_sampler);
	//stride = sizeof(COMBINEDMATRIXCALC_CBUF);

	//g->getDeviceContext()->IASetVertexBuffers( 1, 1, &combined_matrix_watasu_offsetbuffer, &stride, &offset );
	g->getDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	g->getDeviceContext()->IASetIndexBuffer(p->indexbuffer_bill, DXGI_FORMAT_R16_UINT,0);
	g->getDeviceContext()->RSSetState(mss_for_render_bill.rasterstate);
		
	float blendFactor[4] = {1.0f,1.0f,1.0f,1.0f};

	g->getDeviceContext()->OMSetBlendState(mss_for_render_bill.blendstate, blendFactor,0xFFFFFFFF);
	g->getDeviceContext()->VSSetShader(mss_for_render_bill.vs, NULL, 0);
	g->getDeviceContext()->GSSetShader(mss_for_render_bill.gs, NULL, 0);	
	g->getDeviceContext()->PSSetShader(mss_for_render_bill.ps, NULL, 0);
	//	g->getDeviceContext()->VSSetShaderResources(0,1,&->view);
	//	g->getDeviceContext()->VSSetShaderResources(1,1,&matrix_local_texture->view);
	g->getDeviceContext()->DrawIndexed(p->index_count_bill, 0,0);

	g->getDeviceContext()->Flush();
	//	g->getDeviceContext()->PSSetShaderResources(0,1,&testtt);
	//	g->getDeviceContext()->GSSetShader(NULL, NULL,0);

	ID3D11RenderTargetView* t = g->getRenderTargetView();
//	g->getDeviceContext()->OMSetRenderTargets(1, &ggt, gggt);
	g->getDeviceContext()->RSSetViewports(1, g->getViewPort());
	test[0] = NULL;
	test[1] = NULL;
//	{ NULL,NULL};
	g->getDeviceContext()->VSSetConstantBuffers(0,2,test);
	//	g->getDeviceContext()->PSSetShaderResources(0,1,&testtt);
		tt[0] = NULL;
	//	tt[1] = NULL;
		ttt[0] =0;
	//	ttt[1] = 0;
	g->getDeviceContext()->IASetVertexBuffers( 0, 1, tt, ttt, tttt );
	// unset





}


void Texture::renderText(Graphics* g) {

	
	CS::instance()->enter(CS_DEVICECON_CS, "text_render");
	CS::instance()->enter(CS_RENDERDATA_CS, "text_render");

	vector<RenderText*>::iterator it = render_texts.begin();
	while(it != render_texts.end()) {
		RenderText* te = *it;
		if (te->is_use) {
			if (te->is_render) {
				te->text->render(g, te->color,te->x,te->y,te->tex_h, te->width, te->height);
			}
		}
		it++;
	}


	CS::instance()->leave(CS_RENDERDATA_CS, "text_render");
	CS::instance()->leave(CS_DEVICECON_CS, "text_render");

}


void Texture::renderBill(Graphics* g) {
	// 内部でRENDERDATA_CS, DEVICECON_CSを細切れにロックすること // 描画スレッドで呼ぶ
	// texごとにレンダーを呼ぶ
	CS::instance()->enter(CS_RENDERDATA_CS, "texture render");
	// texture情報を取得してから描画にしようするまでロックは外せないがとりあえず取得する
	int part_size = parts.size();
	CS::instance()->leave(CS_RENDERDATA_CS, "texture render");
	CS::instance()->enter(CS_DEVICECON_CS, "texture render");
	CS::instance()->enter(CS_RENDERDATA_CS, "texture render");
	for (int i=0;i<part_size;i++) {
	
		int psize = parts.size();
		if (psize != part_size) {
			// ちょうど変更が入ったタイミング
			// この場合はどうするか
			if (psize > part_size) {
				// 増えただけなので気にしないでまわす
			} else {
				// 減ったので狂うのでリターンする　これで描画されないタイミングがでてくるけどしょうがない

				CS::instance()->leave(CS_RENDERDATA_CS, "texture render");
				CS::instance()->leave(CS_DEVICECON_CS, "texture render");
				return;
			}
		}
		TexturePart* p = parts[i];
		if (p->getIsUse()) {
			if (p->getIsIndexLoad()) {
				// need loadがtruefalseにかかわらず描画する		
				//_renderTex(g, p);
				_renderBill(g,p);
			}
		}
	

	}

	CS::instance()->leave(CS_RENDERDATA_CS, "texture render");
	CS::instance()->leave(CS_DEVICECON_CS, "texture render");
}


void Texture::renderTex(Graphics* g) {

	// 内部でRENDERDATA_CS, DEVICECON_CSを細切れにロックすること // 描画スレッドで呼ぶ
	// texごとにレンダーを呼ぶ
	CS::instance()->enter(CS_RENDERDATA_CS, "texture render");
	// texture情報を取得してから描画にしようするまでロックは外せないがとりあえず取得する
	int part_size = parts.size();
	CS::instance()->leave(CS_RENDERDATA_CS, "texture render");
	CS::instance()->enter(CS_DEVICECON_CS, "texture render");
	CS::instance()->enter(CS_RENDERDATA_CS, "texture render");
	for (int i=0;i<part_size;i++) {
	
		int psize = parts.size();
		if (psize != part_size) {
			// ちょうど変更が入ったタイミング
			// この場合はどうするか
			if (psize > part_size) {
				// 増えただけなので気にしないでまわす
			} else {
				// 減ったので狂うのでリターンする　これで描画されないタイミングがでてくるけどしょうがない

				CS::instance()->leave(CS_RENDERDATA_CS, "texture render");
				CS::instance()->leave(CS_DEVICECON_CS, "texture render");
				return;
			}
		}
		TexturePart* p = parts[i];
		if (p->getIsUse()) {
			if (p->getIsIndexLoad()) {
				// need loadがtruefalseにかかわらず描画する		
				_renderTex(g, p);
				//_renderBill(g,p);
			}
		}
	

	}

	CS::instance()->leave(CS_RENDERDATA_CS, "texture render");
	CS::instance()->leave(CS_DEVICECON_CS, "texture render");
}



void Texture::_render(Graphics* g, int part_size, int p_index) {


	CS::instance()->enter(CS_DEVICECON_CS, "texture render");
	CS::instance()->enter(CS_RENDERDATA_CS, "texture render");
	int psize = parts.size();
	if (psize != part_size) {
		// ちょうど変更が入ったタイミング
		// この場合はどうするか
		if (psize > part_size) {
			// 増えただけなので気にしないでまわす
		} else {
			// 減ったので狂うのでリターンする　これで描画されないタイミングがでてくるけどしょうがない

			CS::instance()->leave(CS_RENDERDATA_CS, "texture render");
			CS::instance()->leave(CS_DEVICECON_CS, "texture render");
			return;
		}
	}
	TexturePart* p = parts[p_index];
	if (p->getIsUse()) {
		if (p->getIsIndexLoad()) {
			// need loadがtruefalseにかかわらず描画する		
			_renderTex(g, p);
			_renderBill(g,p);
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "texture render");
	CS::instance()->leave(CS_DEVICECON_CS, "texture render");



}
void Texture::render(Graphics* g) {
	// 内部でRENDERDATA_CS, DEVICECON_CSを細切れにロックすること // 描画スレッドで呼ぶ
	// texごとにレンダーを呼ぶ
	CS::instance()->enter(CS_RENDERDATA_CS, "texture render");
	// texture情報を取得してから描画にしようするまでロックは外せないがとりあえず取得する
	int psize = parts.size();
	CS::instance()->leave(CS_RENDERDATA_CS, "texture render");
	for (int i=0;i<psize;i++) {
		_render(g, psize, i);
	}
		CS::instance()->enter(CS_DEVICECON_CS, "text_render");
	CS::instance()->enter(CS_RENDERDATA_CS, "text_render");

	vector<RenderText*>::iterator it = render_texts.begin();
	while(it != render_texts.end()) {
		RenderText* te = *it;
		if (te->is_use) {
			if (te->is_render) {
				te->text->render(g, te->color,te->x,te->y,te->tex_h, te->width, te->height);
			}
		}
		it++;
	}


	CS::instance()->leave(CS_RENDERDATA_CS, "text_render");
	CS::instance()->leave(CS_DEVICECON_CS, "text_render");
}	
	
	
void Texture::sendinfoToVertexTexture(Graphics* g) {
	// 内部でRENDERDATA_CS, DEVICECON_CSを細切れにロックすること // 描画補助スレッドで呼ぶ

	// RENDERDATA を細切れにロックしてテクスおよびビルボードのデータを頂点テクスチャに格納する

	TEXTURE_VTEX_STRUCT_TEX str_tex[KTROBO_TEXTURE_VTEX_VERTEXBUFFER_TEX_SIZE];
	TEXTURE_VTEX_STRUCT_BILL str_bill[KTROBO_TEXTURE_VTEX_VERTEXBUFFER_BILL_SIZE];
	memset(str_tex,0,sizeof(TEXTURE_VTEX_STRUCT_TEX)*KTROBO_TEXTURE_VTEX_VERTEXBUFFER_TEX_SIZE);
	memset(str_bill,0,sizeof(TEXTURE_VTEX_STRUCT_BILL)*KTROBO_TEXTURE_VTEX_VERTEXBUFFER_BILL_SIZE);
	int tex_size = 0;
	int bill_size = 0;

	CS::instance()->enter(CS_RENDERDATA_CS, "sendinfotovt");
	int tsize = this->render_texs.size();
	for (int t = 0;t < tsize;t++) {
		RenderTex* tex = render_texs[t];
		if (tex->is_use) {
			if (tex->is_need_load) {
				unsigned int input[] = {
					tex->color,
					(tex->x << 16) + tex->y,
					(tex->width <<16) + tex->height,
					(tex->tex_x << 16)+tex->tex_y,
					(tex->tex_width << 16) + tex->tex_height,
				};
				for (int i=0;i<5;i++) {
					str_tex[tex_size].id = (unsigned short)tex->id;
					str_tex[tex_size].offset = (unsigned short)i;
					str_tex[tex_size].value = input[i];
					tex_size++;
					if (tex_size >= KTROBO_TEXTURE_VTEX_VERTEXBUFFER_TEX_SIZE) {
						// 転送
						CS::instance()->leave(CS_RENDERDATA_CS, "sendinfotovt");
						_sendinfoToVertexTextureTex(g, str_tex, tex_size);
						tex_size = 0;
						memset(str_tex,0,sizeof(TEXTURE_VTEX_STRUCT_TEX)*KTROBO_TEXTURE_VTEX_VERTEXBUFFER_TEX_SIZE);
						CS::instance()->enter(CS_RENDERDATA_CS, "sendinfotovt");
					}
				}
				tex->is_need_load = false;
			}
		}
	}

	if (tex_size > 0 ) {
		CS::instance()->leave(CS_RENDERDATA_CS, "sendinfotovt");
		_sendinfoToVertexTextureTex(g, str_tex, tex_size);
		tex_size = 0;
		memset(str_tex,0,sizeof(TEXTURE_VTEX_STRUCT_TEX)*KTROBO_TEXTURE_VTEX_VERTEXBUFFER_TEX_SIZE);
	} else {
		CS::instance()->leave(CS_RENDERDATA_CS, "sendinfotovt");
	}


// color x y width height tex_x tex_y tex_width tex_height
// 4   2   2  2     2       2      2     2       2  = 20 5テクセル
// color world width height tex_x tex_y tex_width tex_height
// 4    4* 16    4     4      2     2    2          2  = 84  21 テクセル必要
	CS::instance()->enter(CS_RENDERDATA_CS, "sendinfotovt");
	int bsize = this->bill_boards.size();
	for (int b = 0;b < bsize;b++) {
		RenderBillBoard* bil = bill_boards[b];
		if (bil->is_use) {
			if (bil->is_need_load) {
				unsigned int input[] = {
					bil->color,
					0,0,0,0,
					0,0,0,0,
					0,0,0,0,
					0,0,0,0,
					0,0,
					(bil->tex_x << 16)+bil->tex_y,
					(bil->tex_width << 16) + bil->tex_height,
				};
				float input2[] = {
					0.0f,
					bil->world._11,bil->world._12,bil->world._13, bil->world._14,
					bil->world._21,bil->world._22,bil->world._23, bil->world._24,
					bil->world._31,bil->world._32,bil->world._33, bil->world._34,
					bil->world._41,bil->world._42,bil->world._43, bil->world._44,
					bil->width,bil->height,
					0.0f,0.0f,
				};
				unsigned int input3[] = {// 1ならばunsigned int
					1,
					0,0,0,0,
					0,0,0,0,
					0,0,0,0,
					0,0,0,0,
					0,0,
					1,1
				};


				for (int i=0;i<21;i++) {
					str_bill[bill_size].id = (unsigned short)bil->id;
					str_bill[bill_size].flag_with_offset = (unsigned short)(i+ (input3[i] << 8));
					str_bill[bill_size].value_int = input[i];
					str_bill[bill_size].value_float = input2[i];
					bill_size++;
					if (bill_size >= KTROBO_TEXTURE_VTEX_VERTEXBUFFER_BILL_SIZE) {
						// 転送
						CS::instance()->leave(CS_RENDERDATA_CS, "sendinfotovt");
						_sendinfoToVertexTextureBill(g, str_bill, bill_size);
						bill_size = 0;
						memset(str_bill,0,sizeof(TEXTURE_VTEX_STRUCT_BILL)*KTROBO_TEXTURE_VTEX_VERTEXBUFFER_BILL_SIZE);
						CS::instance()->enter(CS_RENDERDATA_CS, "sendinfotovt");
					}
				}
				bil->is_need_load = false;
			}
		}
	}

	if (bill_size > 0 ) {
		CS::instance()->leave(CS_RENDERDATA_CS, "sendinfotovt");
		_sendinfoToVertexTextureBill(g, str_bill, bill_size);
		bill_size = 0;
		memset(str_bill,0,sizeof(TEXTURE_VTEX_STRUCT_BILL)*KTROBO_TEXTURE_VTEX_VERTEXBUFFER_BILL_SIZE);
	} else {
		CS::instance()->leave(CS_RENDERDATA_CS, "sendinfotovt");
	}
	
}
void Texture::updateIndexBuffer(Graphics* g) {

	CS::instance()->enter(CS_RENDERDATA_CS, "createIndexBuffer");
	int p = parts.size();
	for (int i=0;i<p;i++) {
		TexturePart* tex_part = parts[i];
		if (tex_part->getIsUse()) {
			if(tex_part->getIsIndexLoad()) {
				if (tex_part->getIsNeedLoad()) {
					// ロードする
					int in = tex_part->getIndexCount()*3;
					if (in) {
						
						unsigned short* indexs = new unsigned short[in];
						memset(indexs,0,sizeof(unsigned short)*in);
						int temp_count = 0;
						set<int>::iterator it = tex_part->render_tex_ids.begin();
	
						while (it != tex_part->render_tex_ids.end()) {
							int now_id = *it;
							indexs[3*temp_count] = (unsigned short)now_id;
							indexs[3*temp_count+1] = (unsigned short)now_id;
							indexs[3*temp_count+2] = (unsigned short)now_id;
							temp_count++;
							it++;
							if (temp_count*3 >= in) {
								// break;
								break;
							}
						}
						tex_part->index_count_tex = temp_count*3;

						g->getDeviceContext()->UpdateSubresource(tex_part->indexbuffer_tex, 0, 0, (void*)indexs, 0, 0);


						temp_count = 0;
						memset(indexs,0,sizeof(unsigned short)*in);
						it = tex_part->bill_board_ids.begin();
		
						while (it != tex_part->bill_board_ids.end()) {
							int now_id = *it;
							indexs[3*temp_count] = (unsigned short)now_id;
							indexs[3*temp_count+1] = (unsigned short)now_id;
							indexs[3*temp_count+2] = (unsigned short)now_id;
							temp_count++;
							it++;
							if (temp_count*3 >= in) {
								// break;
								break;
							}
						}
						tex_part->index_count_bill = temp_count*3;

						g->getDeviceContext()->UpdateSubresource(tex_part->indexbuffer_bill, 0,0, (void*)indexs, 0, 0);
						delete[] indexs;
					}
					tex_part->setIsNeedLoad(false);
				}
			}
		}
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "createIndexBuffer");
}

void Texture::_createIndexBuffer(Graphics* g, int psize, int p_index) {

	CS::instance()->enter(CS_RENDERDATA_CS, "createIndexBuffer");
	int p = parts.size();
	CS::instance()->leave(CS_RENDERDATA_CS, "createIndexBuffer");

	if (p != psize) {
		// 次の回にまわす
		return;
	}

	CS::instance()->enter(CS_RENDERDATA_CS, "createIndexBuffer");
	TexturePart* tex_part = parts[p_index];
	tex_part->createIndexBuffer(g);
	CS::instance()->leave(CS_RENDERDATA_CS, "createIndexBuffer");
	




}

void TexturePart::createIndexBuffer(Graphics* g) {


	CS::instance()->enter(CS_RENDERDATA_CS, "createindex");
	if (!this->is_index_load && !this->indexbuffer_tex && !this->indexbuffer_bill) {
		// クリエイトする
		
		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.ByteWidth = sizeof(unsigned short) * index_count_max*3;
	    hBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	    hBufferDesc.CPUAccessFlags = 0;
	    hBufferDesc.MiscFlags = 0;
	    hBufferDesc.StructureByteStride = 0;
	    D3D11_SUBRESOURCE_DATA hSubResourceData;

		unsigned short* indexs = new unsigned short[index_count_max*3];
		memset(indexs, 0, sizeof(unsigned short)*index_count_max*3);
	
		set<int>::iterator it = this->render_tex_ids.begin();
		int temp_count = 0;
		while (it != render_tex_ids.end()) {
			int now_id = *it;
			indexs[3*temp_count] = (unsigned short)now_id;
			indexs[3*temp_count+1] = (unsigned short)now_id;
			indexs[3*temp_count+2] = (unsigned short)now_id;
			temp_count++;
			it++;
			if (temp_count >= index_count_max) {
				// break;
				break;
			}
		}
		this->index_count_tex = temp_count*3;
	    hSubResourceData.pSysMem = indexs;
	    hSubResourceData.SysMemPitch = 0;
	    hSubResourceData.SysMemSlicePitch = 0;
		HRESULT hr = g->getDevice()->CreateBuffer( &hBufferDesc, &hSubResourceData, &this->indexbuffer_tex );
	    if( FAILED( hr ) ) {
	       delete[] indexs;
		   	CS::instance()->leave(CS_RENDERDATA_CS, "createindex");
		   throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "index buffer make error");
		}

		delete[] indexs;


		hBufferDesc;
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.ByteWidth = sizeof(unsigned short) * index_count_max*3;
	    hBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	    hBufferDesc.CPUAccessFlags = 0;
	    hBufferDesc.MiscFlags = 0;
	    hBufferDesc.StructureByteStride = 0;
	    hSubResourceData;

		indexs = new unsigned short[index_count_max*3];
		memset(indexs,0,sizeof(unsigned short)*index_count_max*3);
		it = this->bill_board_ids.begin();
		temp_count = 0;
		while (it != bill_board_ids.end()) {
			int now_id = *it;
			indexs[3*temp_count] = (unsigned short)now_id;
			indexs[3*temp_count+1] = (unsigned short)now_id;
			indexs[3*temp_count+2] = (unsigned short)now_id;
			temp_count++;
			it++;
			if (temp_count >= index_count_max) {
				// break;
				break;
			}
		}
		this->index_count_bill = temp_count*3;

	    hSubResourceData.pSysMem = indexs;
	    hSubResourceData.SysMemPitch = 0;
	    hSubResourceData.SysMemSlicePitch = 0;
		hr = g->getDevice()->CreateBuffer( &hBufferDesc, &hSubResourceData, &this->indexbuffer_bill );
	    if( FAILED( hr ) ) {
	       delete[] indexs;
		   CS::instance()->leave(CS_RENDERDATA_CS, "createindex");
		   throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "index buffer make error");
		}

		delete[] indexs;

		is_index_load = true;
		is_need_load = true;

	} 
	CS::instance()->leave(CS_RENDERDATA_CS, "createindex");


}


void Texture::createIndexBuffer(Graphics* g) {
	// ロードスレッドで呼ぶ
	
	CS::instance()->enter(CS_RENDERDATA_CS, "createIndexBuffer");
	int psize = parts.size();
	CS::instance()->leave(CS_RENDERDATA_CS, "createIndexBuffer");


	for (int i=0;i<psize;i++) {
		_createIndexBuffer(g, psize, i);
	}

}

#define KTROBO_TEXTURE_DELETEDAYO_ONELOOP_SAKUJYOKAISUU 50

void Texture::deletedayo(){
	// delete処理を行う　ロードスレッドで呼ぶ 細切れにdeleteする?たぶん描画がされていないときとか（ロード画面？画面遷移画面などで呼ぶことになりそう)



	CS::instance()->enter(CS_RENDERDATA_CS, "deleteall");
	if (is_all_delete) {
		// 全消去処理を呼ぶ

		Release();
		CS::instance()->leave(CS_RENDERDATA_CS, "deleteall");
		return;
	} 
	CS::instance()->leave(CS_RENDERDATA_CS, "deleteall");

	for (int i=0;i < KTROBO_TEXTURE_DELETEDAYO_ONELOOP_SAKUJYOKAISUU;i++) {
		CS::instance()->enter(CS_RENDERDATA_CS, "deleteall");

		int size = erase_render_tex_ids.size();
		if (size > 0) {
			int render_tex_id = *erase_render_tex_ids.begin();
			erase_render_tex_ids.erase(render_tex_id);
			if (render_tex_indexs.find(render_tex_id) != render_tex_indexs.end()) {
				RenderTex* t = render_texs[render_tex_indexs[render_tex_id]];
				render_texs[render_tex_indexs[render_tex_id]] = &dummy_rendertex;
		
				// 削除処理を行う
				TexturePart* p = parts[t->tex_index];
				p->eraseRenderTexId(t->id);
				render_tex_indexs.erase(render_tex_id);
				unuse_render_tex_ids.insert(render_tex_id);

				delete t;
				t = 0;
			}
		} else {
			CS::instance()->leave(CS_RENDERDATA_CS, "deleteall");
			break;
		}

		CS::instance()->leave(CS_RENDERDATA_CS, "deleteall");
	}

	for (int i=0;i < KTROBO_TEXTURE_DELETEDAYO_ONELOOP_SAKUJYOKAISUU;i++) {
		CS::instance()->enter(CS_RENDERDATA_CS, "deleteall");

		int size = erase_bill_board_ids.size();
		if (size > 0) {
			int bill_id = *erase_bill_board_ids.begin();
			erase_bill_board_ids.erase(bill_id);
			if (bill_board_indexs.find(bill_id) != bill_board_indexs.end()) {
				RenderBillBoard* t = bill_boards[bill_board_indexs[bill_id]];
				bill_boards[bill_board_indexs[bill_id]] = &dummy_billboard;
		
				// 削除処理を行う
				TexturePart* p = parts[t->tex_index];
				p->eraseRenderTexId(t->id);
				bill_board_indexs.erase(bill_id);
				unuse_bill_board_ids.insert(bill_id);
				delete t;
				t = 0;
			}
		} else {
			CS::instance()->leave(CS_RENDERDATA_CS, "deleteall");
			break;
		}

		CS::instance()->leave(CS_RENDERDATA_CS, "deleteall");
	}
}


ID3D11Buffer* Texture::render_vertexbuffer = 0;
ID3D11Buffer* Texture::vertextex_vertexbuffer_tex=0;
ID3D11Buffer* Texture::vertextex_vertexbuffer_bill=0;
ID3D11Buffer* Texture::vertextex_indexbuffer = 0;
ID3D11Buffer* Texture::cbuf1_buffer = 0;
ID3D11Buffer* Texture::cbuf2_buffer = 0;
TEXTURE_BILL_CBUF Texture::cbuf2;
TEXTURE_TEX_CBUF Texture::cbuf1;




MYSHADERSTRUCT Texture::mss_for_render_tex;
MYSHADERSTRUCT Texture::mss_for_render_bill;
MYSHADERSTRUCT Texture::mss_for_vertextex_tex;
MYSHADERSTRUCT Texture::mss_for_vertextex_bill;




void Texture::updateCBuf1(Graphics* g, TexturePart* part) {

	cbuf1.tex_height = part->getClass()->height;
	cbuf1.tex_width = part->getClass()->width;

	CS::instance()->enter(CS_RENDERDATA_CS, "updatecbuf1");
	g->getDeviceContext()->UpdateSubresource(cbuf1_buffer,0,0,(void*)&cbuf1,0,0);
	CS::instance()->leave(CS_RENDERDATA_CS, "updatecbuf1");
}


void Texture::Init(Graphics* g) {

	// renderのためのvertexbufferを作る

	D3D11_BUFFER_DESC bd;
	memset(&bd,0,sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(TEXTURE_RENDER_STRUCT)*KTROBO_TEXTURE_RENDER_VERTEXBUFFER_SIZE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	
	D3D11_SUBRESOURCE_DATA hSubResourceData;

	unsigned short* indexs = new unsigned short[KTROBO_TEXTURE_RENDER_VERTEXBUFFER_SIZE];//index_count_max*3];
	memset(indexs, 0, sizeof(unsigned short)*KTROBO_TEXTURE_RENDER_VERTEXBUFFER_SIZE);
	for (int i=0;i<KTROBO_TEXTURE_RENDER_VERTEXBUFFER_SIZE;i++) {
		indexs[i] = i;
	}
	hSubResourceData.pSysMem = indexs;
	hSubResourceData.SysMemPitch = 0;
    hSubResourceData.SysMemSlicePitch = 0;
	HRESULT hr = g->getDevice()->CreateBuffer(&bd, &hSubResourceData ,&(render_vertexbuffer));
	if (FAILED(hr)) {
		delete[] indexs;
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
	}
	delete[] indexs;
	// viewを作る

	// vertextextureのためのvertexbufferを作る

	
	bd;
	memset(&bd,0,sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(TEXTURE_VTEX_STRUCT_TEX)*KTROBO_TEXTURE_VTEX_VERTEXBUFFER_TEX_SIZE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	
	hr = g->getDevice()->CreateBuffer(&bd, NULL ,&(vertextex_vertexbuffer_tex));
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
	}
	
	
	bd;
	memset(&bd,0,sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(TEXTURE_VTEX_STRUCT_BILL)*KTROBO_TEXTURE_VTEX_VERTEXBUFFER_BILL_SIZE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	
	hr = g->getDevice()->CreateBuffer(&bd, NULL ,&(vertextex_vertexbuffer_bill));
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vertex buffer make error");;
	}

	// indexbufferを作る

	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	int index_count_max;

	if (KTROBO_TEXTURE_VTEX_VERTEXBUFFER_TEX_SIZE > KTROBO_TEXTURE_VTEX_VERTEXBUFFER_BILL_SIZE) {
		index_count_max = KTROBO_TEXTURE_VTEX_VERTEXBUFFER_TEX_SIZE;
	} else {
		index_count_max = KTROBO_TEXTURE_VTEX_VERTEXBUFFER_BILL_SIZE;
	}
	hBufferDesc.ByteWidth = sizeof(unsigned short) * index_count_max*3;
	hBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = 0;
	hSubResourceData;
	{
	unsigned short* indexs = new unsigned short[index_count_max*3];
	memset(indexs, 0, sizeof(unsigned short)*index_count_max*3);
	
	for (int i=0; i <index_count_max; i++) {
		indexs[3*i] = i;
		indexs[3*i+1] = i;
		indexs[3*i+2] = i;
	}
	
	hSubResourceData.pSysMem = indexs;
	hSubResourceData.SysMemPitch = 0;
	hSubResourceData.SysMemSlicePitch = 0;
	hr = g->getDevice()->CreateBuffer( &hBufferDesc, &hSubResourceData, &vertextex_indexbuffer);
	if( FAILED( hr ) ) {
	  delete[] indexs;
	  CS::instance()->leave(CS_RENDERDATA_CS, "createindex");
	  throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "index buffer make error");
    }

	delete[] indexs;
	}


	// 各種shaderstructのロード

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"BILL_ID", 0, DXGI_FORMAT_R16_UINT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	
	loadShader(g, &mss_for_render_bill, KTROBO_TEXTURE_SHADER_FILENAME_RENDER_BILL, KTROBO_TEXTURE_SHADER_VS, KTROBO_TEXTURE_SHADER_GS,
		KTROBO_TEXTURE_SHADER_PS, g->getScreenWidth(), g->getScreenHeight(),
								layout, 1, true);

	D3D11_INPUT_ELEMENT_DESC layout2[] = {
		{"TEX_ID", 0, DXGI_FORMAT_R16_UINT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	
	loadShader(g, &mss_for_render_tex, KTROBO_TEXTURE_SHADER_FILENAME_RENDER_TEX, KTROBO_TEXTURE_SHADER_VS, KTROBO_TEXTURE_SHADER_GS,
		KTROBO_TEXTURE_SHADER_PS, g->getScreenWidth(), g->getScreenHeight(),
								layout2, 1, true);



	/*
	unsigned short id;
	unsigned short offset;
	unsigned int value;
	*/
	D3D11_INPUT_ELEMENT_DESC layout3[] = {
		{"TEX_ID", 0, DXGI_FORMAT_R16_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"OFFSET", 0, DXGI_FORMAT_R16_UINT, 0, 2, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"VALUE", 0, DXGI_FORMAT_R32_UINT, 0, 4, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	
	loadShader(g, &mss_for_vertextex_tex, KTROBO_TEXTURE_SHADER_FILENAME_VERTEXTEXTURE_TEX, KTROBO_TEXTURE_SHADER_VS, KTROBO_TEXTURE_SHADER_GS,
		KTROBO_TEXTURE_SHADER_PS, KTROBO_TEXTURE_VTEX_WIDTH_HEIGHT, KTROBO_TEXTURE_VTEX_WIDTH_HEIGHT, layout3, 3, false);

	/*

	unsigned short id;
	unsigned short flag_with_offset; // 上位8ビットでvalue_floatかvalue_intかの判定を行う
	float value_float;
	unsigned int value_int;
	// ifが必要になる

	*/
	D3D11_INPUT_ELEMENT_DESC layout4[] = {
		{"BILL_ID", 0, DXGI_FORMAT_R16_UINT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"OFFSET", 0, DXGI_FORMAT_R16_UINT, 0, 2, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"V_FLOAT", 0, DXGI_FORMAT_R32_FLOAT, 0, 4, D3D11_INPUT_PER_VERTEX_DATA,0},
		{"V_INT", 0, DXGI_FORMAT_R32_UINT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	
	loadShader(g, &mss_for_vertextex_bill, KTROBO_TEXTURE_SHADER_FILENAME_VERTEXTEXTURE_BILL, KTROBO_TEXTURE_SHADER_VS, KTROBO_TEXTURE_SHADER_GS,
		KTROBO_TEXTURE_SHADER_PS, KTROBO_TEXTURE_VTEX_WIDTH_HEIGHT, KTROBO_TEXTURE_VTEX_WIDTH_HEIGHT, layout4, 4, false);

	// コンスタントバッファの作成

	D3D11_BUFFER_DESC des;
	des.ByteWidth = sizeof(TEXTURE_TEX_CBUF);
	des.Usage = D3D11_USAGE_DEFAULT;
	des.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	des.CPUAccessFlags = 0;//D3D11_CPU_ACCESS_WRITE;
	des.MiscFlags = 0;
	des.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA idat;
	memset(&cbuf1, 0, sizeof(TEXTURE_TEX_CBUF));
	cbuf1.screen_height = g->getScreenHeight();
	cbuf1.screen_width = g->getScreenWidth();
	cbuf1.vtex_width = KTROBO_TEXTURE_VTEX_WIDTH_HEIGHT;
	cbuf1.vtex_height = KTROBO_TEXTURE_VTEX_WIDTH_HEIGHT;
	cbuf1.tex_height = KTROBO_TEXTURE_VTEX_WIDTH_HEIGHT;
	cbuf1.tex_width = KTROBO_TEXTURE_VTEX_WIDTH_HEIGHT;
	cbuf1.offset = 0;
	cbuf1.offset2 = 0;
	idat.pSysMem = &cbuf1;
	idat.SysMemPitch = 0;
	idat.SysMemSlicePitch = 0;
	hr = g->getDevice()->CreateBuffer(&des, &idat, &cbuf1_buffer);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cbuf make error");
	}

	des;
	des.ByteWidth = sizeof(TEXTURE_BILL_CBUF);
	des.Usage =D3D11_USAGE_DEFAULT;// D3D11_USAGE_DYNAMIC;
	des.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	des.CPUAccessFlags = 0;//D3D11_CPU_ACCESS_WRITE;
	des.MiscFlags = 0;
	des.StructureByteStride = 0;

	idat;
	memset(&cbuf2, 0, sizeof(TEXTURE_BILL_CBUF));
	MyMatrixIdentity(cbuf2.view);
	MyMatrixIdentity(cbuf2.proj);
	idat.pSysMem = &cbuf2;
	idat.SysMemPitch = 0;
	idat.SysMemSlicePitch = 0;
	hr = g->getDevice()->CreateBuffer(&des, &idat, &cbuf2_buffer);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cbuf make error");
	}

}

void Texture::setViewProj(Graphics* g, MYMATRIX* view, MYMATRIX* proj, MYVECTOR3* from, MYVECTOR3* at) {



	DWORD color = 0xFFFFFFFF;

	
	MYVECTOR3 vec;
	MYVECTOR3 yvec(0,-1,0);
	MYVECTOR3 temp_vec;
	MYVECTOR3 axis;
	MYMATRIX ma;
	MYMATRIX ma2;

	temp_vec = *from - *at;

	MyVec3Normalize(vec,temp_vec);
	vec.float3.z = 0;
	MyVec3Normalize(vec,vec);
	MyVec3Cross(axis, yvec, vec);
	if ((abs(axis.float3.x) < 0.00001f) && (abs(axis.float3.y) <0.00001f) &&( abs(axis.float3.z) <0.00001f)) {
		axis = MYVECTOR3(0,0,1);
	}
	float th;
	MyVec3Normalize(axis,axis);

	th = acos(MyVec3Dot(vec,yvec));
	MyMatrixRotationAxis(ma,axis,th);





	MyVec3Normalize(vec,temp_vec);
	float len = MyVec3Length(vec);

	/*
	bool is_z_p=true;
	if (vec.float3.z >=0) {
		is_z_p = false;
	}else {
	}
	vec.float3.z = 0;
	MYVECTOR3 dup(0,0,-1);
	MyVec3Cross(axis,vec,dup);
	if ((abs(axis.float3.x) < 0.00001f) && (abs(axis.float3.y) <0.00001f) &&( abs(axis.float3.z) <0.00001f)) {
		axis = MYVECTOR3(0,1,0);
	}
	*/


	MYVECTOR3 temp_yvec = vec;
	temp_yvec.float3.z = 0;
	float len2 = MyVec3Length(temp_yvec);
	float the = acos(len2/len);
	MyVec3Cross(axis, temp_yvec,vec);
	MyVec3Normalize(axis,axis);
	if ((abs(axis.float3.x) < 0.00001f) && (abs(axis.float3.y) <0.00001f) &&( abs(axis.float3.z) <0.00001f)) {
		axis = MYVECTOR3(1,0,0);
	}
	MyMatrixRotationAxis(ma2,axis,the);




	/*
	if (is_z_p) {
		MyMatrixRotationAxis(ma2, axis, 3.14+1.57-the);
	}else {
		MyMatrixRotationAxis(ma2, axis, 1.57+the);
	}
	*/


	MyMatrixMultiply(ma,ma,ma2);
//	MyMatrixMultiply(ma,ma,*view);
//	MYMATRIX ma;
//	MyMatrixIdentity(ma);
	cbuf2.proj = *proj;
	cbuf2.view = *view;
	cbuf2.mym = ma;
	MYVECTOR3 test(1,0,1);
	MYVECTOR3 test2(-1,0,-1);
	MyVec3TransformCoord(test,test,ma);
	MyVec3TransformCoord(test,test,*view);
	MyVec3TransformCoord(test,test,*proj);
	MyVec3TransformCoord(test2,test2,ma);
	MyVec3TransformCoord(test2,test2,*view);
	MyVec3TransformCoord(test2,test2,*proj);
	

	D3D11_MAPPED_SUBRESOURCE subresource;

	CS::instance()->enter(CS_DEVICECON_CS, "setviewproj");
	//g->getDeviceContext()->Map(cbuf2_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	//memcpy( subresource.pData, &cbuf2, sizeof(TEXTURE_BILL_CBUF) );
	//g->getDeviceContext()->Unmap(cbuf2_buffer, 0);
	g->getDeviceContext()->UpdateSubresource(cbuf2_buffer,0,NULL,&cbuf2,0,0);
	CS::instance()->leave(CS_DEVICECON_CS, "setviewproj");
}


void Texture::Del() {

	mss_for_render_bill.Del();
	mss_for_render_tex.Del();
	mss_for_vertextex_tex.Del();
	mss_for_vertextex_bill.Del();

	if (vertextex_vertexbuffer_tex) {
		vertextex_vertexbuffer_tex->Release();
		vertextex_vertexbuffer_tex = 0;
	}

	if (vertextex_vertexbuffer_bill) {
		vertextex_vertexbuffer_bill->Release();
		vertextex_vertexbuffer_bill = 0;
	}



	if (render_vertexbuffer) {
		render_vertexbuffer->Release();
		render_vertexbuffer = 0;
	}

	if (vertextex_indexbuffer) {
		vertextex_indexbuffer->Release();
		vertextex_indexbuffer = 0;
	}

	if (cbuf1_buffer) {
		cbuf1_buffer->Release();
		cbuf1_buffer = 0;
	}

	if (cbuf2_buffer) {
		cbuf2_buffer->Release();
		cbuf2_buffer =0; 
	}

}


void Texture::loadShader(Graphics* g, MYSHADERSTRUCT* s, char* shader_filename, char* vs_func_name, char* gs_func_name,
								char* ps_func_name, unsigned int ds_width,unsigned int ds_height,
								D3D11_INPUT_ELEMENT_DESC* layout, int numoflayout, bool blend_enable) {

	HRESULT hr = S_OK;
	ID3DBlob* pblob = 0;
	memset(s, 0, sizeof(MYSHADERSTRUCT));

	try {
		CompileShaderFromFile(shader_filename, vs_func_name, "vs_4_0", &pblob);
		hr = g->getDevice()->CreateVertexShader(pblob->GetBufferPointer(),
			pblob->GetBufferSize(),
			NULL,
			&s->vs);
		if (FAILED(hr)) {
			pblob->Release();
			pblob = 0;
			s->vs = 0;
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "vs make error");;
		}

		// 入力レイアウトの定義
		
		
		unsigned int num = numoflayout;//sizeoflayout/ sizeof(layout[0]);
			
		hr = g->getDevice()->CreateInputLayout(
			layout,num,pblob->GetBufferPointer(),
			pblob->GetBufferSize(),
			&s->vertexlayout);
		if (FAILED(hr)) {
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "layout make error");;
		}
		if (pblob) {
			pblob->Release();
			pblob = 0;
		}
		 // Set the input layout
		
		CompileShaderFromFile(shader_filename, gs_func_name, "gs_4_0", &pblob);
		hr = g->getDevice()->CreateGeometryShader(pblob->GetBufferPointer(),
			pblob->GetBufferSize(), NULL, &s->gs);
		if (FAILED(hr)) {
			pblob->Release();
			pblob = 0;
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "gs make error");;
		}
		if (pblob) {
			pblob->Release();
			pblob = 0;
		}

		CompileShaderFromFile(shader_filename, ps_func_name, "ps_4_0", &pblob);
		hr = g->getDevice()->CreatePixelShader(pblob->GetBufferPointer(),
			pblob->GetBufferSize(),
			NULL,&s->ps);
		if (FAILED(hr)) {
			pblob->Release();
			pblob = 0;
			s->ps = 0;
			throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "ps make error");;
		}

		if (pblob) {
			pblob->Release();
			pblob = 0;
		}

		D3D11_BLEND_DESC BlendDesc;
		memset(&BlendDesc, 0, sizeof(BlendDesc));
		BlendDesc.AlphaToCoverageEnable =false;// blend_enable;
		BlendDesc.IndependentBlendEnable = blend_enable;
		BlendDesc.RenderTarget[0].BlendEnable = blend_enable;
		BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;//SRC_ALPHA;
		BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;//INV_SRC_ALPHA;
		if (blend_enable) {
			BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		}
		BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].SrcBlendAlpha =D3D11_BLEND_ONE;
		BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		HRESULT hr = g->getDevice()->CreateBlendState(&BlendDesc,&s->blendstate);
		if (FAILED(hr)) {
			throw new GameError(KTROBO::FATAL_ERROR, "failed in blend state create");
		}
		/*
		 BlendDesc.IndependentBlendEnable = FALSE;
   // アルファブレンドを無効
   BlendDesc.RenderTarget[0].BlendEnable = FALSE;
   BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
   BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
   BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
   BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
   BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
   BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
   BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
   hr = pD3DDevice->CreateBlendState( &BlendDesc, &m_Step01.pBlendState );
   if( FAILED( hr ) )
   */
	
	} catch (KTROBO::GameError* e) {
		if (pblob) {
			pblob->Release();
			pblob = 0;
		}

		Del();

		
	//	MessageBoxA(NULL,e->getMessage(), "shader compile error", MB_OK);
		throw e;
	}


	D3D11_RASTERIZER_DESC _rasterDesc;
	_rasterDesc.AntialiasedLineEnable = true; // changed to true
	_rasterDesc.CullMode = D3D11_CULL_NONE;
	_rasterDesc.DepthBias = 0;
	_rasterDesc.DepthBiasClamp = 0.0f;
	_rasterDesc.DepthClipEnable = false;
	_rasterDesc.FillMode = D3D11_FILL_SOLID;
	_rasterDesc.FrontCounterClockwise = false;
	_rasterDesc.MultisampleEnable = true; // changed to true
	_rasterDesc.ScissorEnable = false;
	_rasterDesc.SlopeScaledDepthBias = 0.0f;
	hr  = g->getDevice()->CreateRasterizerState( &_rasterDesc, &s->rasterstate );
	if( FAILED( hr ) ) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "raster make error");;
	}

/*
	D3D11_BUFFER_DESC des;
	des.ByteWidth = sizeof(TelopTextsCBuf1);
	des.Usage = D3D11_USAGE_DEFAULT;
	des.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	des.CPUAccessFlags = 0;
	des.MiscFlags = 0;
	des.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA idat;
	idat.pSysMem = &this->cbuf1;
	idat.SysMemPitch = 0;
	idat.SysMemSlicePitch = 0;
	hr = g->getDevice()->CreateBuffer(&des, &idat, &cbuf1_buffer);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cbuf make error");
	}

	des.ByteWidth = sizeof(TelopTextsCBuf2);
	des.Usage = D3D11_USAGE_DEFAULT;
	des.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	des.CPUAccessFlags = 0;
	des.MiscFlags = 0;
	des.StructureByteStride = 0;

//	D3D11_SUBRESOURCE_DATA idat;
	idat.pSysMem = &this->cbuf2;
	idat.SysMemPitch = 0;
	idat.SysMemSlicePitch = 0;
	hr = g->getDevice()->CreateBuffer(&des, &idat, &cbuf2_buffer);
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "cbuf make error");
	}
*/

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof(descDepth) );
	descDepth.Width = ds_width; descDepth.Height = ds_height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1; descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
 
	//ID3D11Texture2D* pDepthStencil = 0;
	hr = g->getDevice()->CreateTexture2D( &descDepth, NULL, &s->depthstencil );
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "depth stencil make error");
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC descDepthStencilView;
	ZeroMemory( &descDepthStencilView, sizeof(descDepthStencilView) );
	descDepthStencilView.Format = descDepth.Format;
	descDepthStencilView.ViewDimension =  D3D11_DSV_DIMENSION_TEXTURE2D;
	descDepthStencilView.Texture2D.MipSlice = 0;
	//ID3D11DepthStencilView* pDepthStencilView = 0;
	hr = g->getDevice()->CreateDepthStencilView( s->depthstencil, &descDepthStencilView, &s->depthstencilview );
	if (FAILED(hr)) {
		Del();
		throw new KTROBO::GameError(KTROBO::FATAL_ERROR, "depth stencil make error");
	}


}






Texture* Textures::getInstance(int index) {
	int size = texs.size();

	if (size) {
		if (index ==0) {
		return texs[0];
		} else {
			return texs[1];
		}
	} else {
		makeInst();
		if (index ==0) {
		return texs[0];
		} else {
			return texs[1];
		}
	}

	throw new GameError(KTROBO::WARNING, "no texture");


}

ITexture* Textures::getInterface(int index) {
	int size = texs.size();
	if (size) {
		if (index == 0) {
		return texs[0];
		} else {
			return texs[1];
		}

	} else {
		makeInst();
		return texs[0];
	}
	throw new GameError(KTROBO::WARNING, "no texture");

}
int Textures::makeInst() {

	int size = texs.size();
	if (size) {
		return 0;
	}

	Texture* t = new Texture(loader);
	Texture* t2 = new Texture(loader);
	texs.push_back(t);
	texs.push_back(t2);
	return 0;
}

Textures::Textures(MyTextureLoader* l) {
	loader = l;
	makeInst();
}
Textures::~Textures() {
	int size = texs.size();
	if (size) {
		texs[0]->Release();
		delete texs[0];
		texs[1]->Release();
		delete texs[1];
		texs.clear();
	}
}

void Textures::render(Graphics* g, int index) {
	// 内部でRENDERDATA_CS, DEVICECON_CSを細切れにロックすること // 描画スレッドで呼ぶ
	int size = texs.size();
	if (size) {
		if (index ==0) {
			texs[0]->render(g);
		} else {
			g->getDeviceContext()->ClearDepthStencilView(Mesh::pDepthStencilView,  D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,1.0f, 0 );
			texs[1]->render(g);
		}
	}
}
void Textures::sendinfoToVertexTexture(Graphics* g) {
	// 内部でRENDERDATA_CS, DEVICECON_CSを細切れにロックすること // 描画補助スレッドで呼ぶ
	int size = texs.size();
	if (size) {
		texs[0]->sendinfoToVertexTexture(g);
		texs[1]->sendinfoToVertexTexture(g);
	}
}



void Textures::updateIndexBuffer(Graphics* g) {
	//描画補助スレッドで呼ぶ
	int size= texs.size();
	if (size) {
		texs[0]->updateIndexBuffer(g);
		texs[1]->updateIndexBuffer(g);
	}
}

void Textures::createIndexBuffer(Graphics* g) {
	// ロードスレッドで呼ぶ 適正な初期値でcreateする
	int size = texs.size();
	if (size) {
		texs[0]->createIndexBuffer(g);
		texs[1]->createIndexBuffer(g);
	}
}
void Textures::deletedayo() {
	// delete処理を行う　ロードスレッドで呼ぶ 細切れにdeleteする
	int size = texs.size();
	if (size) {
		texs[0]->deletedayo();
		texs[1]->deletedayo();
	}
}

int Texture::getRenderText(char* t, int x, int y, int tex_h, int width, int height) {
	CS::instance()->enter(CS_RENDERDATA_CS, "makerenderText");

	stringconverter sc;
	WCHAR buf[512];
	memset(buf,0,sizeof(WCHAR)*512);
	sc.charToWCHAR(t, buf);

	// 空いている場所があるか調べる
	if (render_texts_erased.size()) {
		int inde = *render_texts_erased.begin();
		render_texts[inde]->is_use = true;
		render_texts[inde]->height = height;
		render_texts[inde]->is_render = false;
		render_texts[inde]->color = 0xFFFFFFFF;
		render_texts[inde]->text->changeText(buf,wcslen(buf));
		render_texts[inde]->tex_h = tex_h;
		render_texts[inde]->width = width;
		render_texts[inde]->x = x;
		render_texts[inde]->y = y;
		render_texts_erased.erase(inde);
		CS::instance()->leave(CS_RENDERDATA_CS, "makerendertext");	
		return inde;
	}

	Text* te = new Text(buf, wcslen(buf));
	RenderText* rt = new RenderText();
	rt->color = 0xFFFFFFFF;
	rt->is_render = false;
	rt->is_use = true;
	rt->text = te;
	rt->x = x;
	rt->y = y;
	rt->height = height;
	rt->width  = width;
	rt->tex_h = tex_h;
	int rttt = render_texts.size();
	render_texts.push_back(rt);

	CS::instance()->leave(CS_RENDERDATA_CS, "makerenderText");
	return rttt;
}
void Texture::setRenderTextColor(int text_id, unsigned int color) {
	CS::instance()->enter(CS_RENDERDATA_CS, "set text color");
	if (render_texts.size() > text_id  && text_id >=0) {
		render_texts[text_id]->color = color;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "set text color");
}
void Texture::setRenderTextIsRender(int text_id, bool t) {
	CS::instance()->enter(CS_RENDERDATA_CS, "set text render");
	if (render_texts.size() > text_id  && text_id >=0) {
		render_texts[text_id]->is_render = t;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "set text render");
}

void Texture::lightdeleteRenderText(int text_id) {

	CS::instance()->enter(CS_RENDERDATA_CS, "set text light delete");
	if (render_texts.size() > text_id  && text_id >=0) {
		render_texts[text_id]->is_use = false;
		render_texts_erased.insert(text_id);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "set text light delete");
}


void Texture::lightdeleteAllRenderText() {

	CS::instance()->enter(CS_RENDERDATA_CS, "set text light delete");
	int ss = render_texts.size();
	for (int i=0;i<ss;i++) {
		lightdeleteRenderText(i);
	}

	CS::instance()->leave(CS_RENDERDATA_CS, "set text light delete");
}

void Texture::setRenderTextPos(int text_id, int x, int y) {
	CS::instance()->enter(CS_RENDERDATA_CS, "set text pos");
	if (render_texts.size() > text_id  && text_id >=0) {
		render_texts[text_id]->x = x;
		render_texts[text_id]->y = y;
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "set text pos");
}
void Texture::setRenderTextChangeText(int text_id, char* t) {
	CS::instance()->enter(CS_RENDERDATA_CS, "set text change");
	if (render_texts.size() > text_id  && text_id >=0) {
		WCHAR buf[512];
		memset(buf,0,sizeof(WCHAR)*512);
		stringconverter sc;
		sc.charToWCHAR(t, buf);
		render_texts[text_id]->width = wcslen(buf) * render_texts[text_id]->tex_h;
		render_texts[text_id]->text->changeText(buf, wcslen(buf));
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "set text change");
}

float Texture::getRenderTextWidth(int text_id, int height) {
	float ans = 0;
	CS::instance()->enter(CS_RENDERDATA_CS, "set text change");
	if (render_texts.size() > text_id  && text_id >=0) {
		ans = render_texts[text_id]->text->getWidth((float)height);
	}
	CS::instance()->leave(CS_RENDERDATA_CS, "set text change");

	return ans;
}
