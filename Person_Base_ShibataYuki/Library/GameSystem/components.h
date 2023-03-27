//=========================================================
// ｺﾝﾎﾟｰﾈﾝﾄ追加・保存用のヘッダー
//=========================================================

//--- インクルード部
// GameSystem
#include <GameSystem/Component/cursor.h>
#include <GameSystem/Component/actionPlayer.h>

#include <GameSystem/Component/Transform/transform.h>
#include <GameSystem/Component/Transform/rectTransform.h>
#include <GameSystem/Component/Transform/rigidbody.h>

#include <GameSystem/Component/Collision/collision.h>
#include <GameSystem/Component/Collision/sphereCollision.h>
#include <GameSystem/Component/Collision/boxCollision.h>

#include <GameSystem/Component/Camera/camera.h>
#include <GameSystem/Component/Camera/gameCamera.h>
#include <GameSystem/Component/Camera/stackCamera.h>
#include <GameSystem/Component/Camera/debugCamera.h>

#include <GameSystem/Component/Light/light.h>
#include <GameSystem/Component/Light/directionalLight.h>

#include <GameSystem/Component/Renderer/renderer.h>
#include <GameSystem/Component/Renderer/polygonRenderer.h>
#include <GameSystem/Component/Renderer/billboardRenderer.h>
#include <GameSystem/Component/Renderer/meshRenderer.h>
#include <GameSystem/Component/Renderer/boxRenderer.h>
#include <GameSystem/Component/Renderer/sphereRenderer.h>
#include <GameSystem/Component/Renderer/modelRenderer.h>
#include <GameSystem/Component/Renderer/textRenderer.h>
#include <GameSystem/Component/Renderer/effekseerRenderer.h>
#include <GameSystem/Component/Renderer/volume.h>

//--- serealize, AddComponent するｺﾝﾎﾟｰﾈﾝﾄを追加
#include <AISystem/System/aiSystem.h>
#include <AISystem/System/aiAgent.h>

void CreateComponentType();