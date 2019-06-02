#include "CCore.h"

extern CCore* core;

CModControl::CModControl()
{
    this->p_bIsActive = false;
    this->p_bBlockInput = false;
    this->p_bControlState = CMOD_FREECAM;
    this->pickedPoint = -1;
    this->pickedPointSecond = -1;

    // player
    this->playingID = 0;
    this->playingTime = 1;
    this->playintSpeed = 100.0f;

}

bool CModControl::IsActive() { return this->p_bIsActive; }

void CModControl::OnVKKey(USHORT key)
{
    if (key == VK_F1) {
        if (core->getGame()->IsGameRunning())
            this->ToggleMod();
        // core->SetGamePhysic(!core->IsGamePhysicRunning());
    } else if (key == VK_F7) {
        core->SetGamePhysic(!core->IsGamePhysicRunning());
    }

    if (this->p_bIsActive) {
        float Camspeed = 1.0f;
        if (GetKeyState(VK_LSHIFT) && 0x8000)
            Camspeed = 5.0f;

        switch (key) {
        case VK_F2: {
            if (this->p_bIsActive) {
                this->p_bControlState = CMOD_FREECAM;
            }
        } break;
        case VK_F3: {
            if (this->p_bIsActive) {
                this->p_bControlState = CMOD_PICKING;
            }
        } break;
        case VK_F4: {
            if (this->p_bIsActive) {
                CamPoint point;
                point.position = this->p_vectCamera;
                point.rotation = this->p_vectCameraRot;
                this->points.push_back(point);

            }
        } break;
        case VK_F5: {
            if (this->p_bIsActive) {
                if (this->pickedPoint != -1 && this->pickedPointSecond != -1) {
                    CamPath newpath;
                    newpath.pointA = this->pickedPoint;
                    newpath.pointB = this->pickedPointSecond;
                    newpath.lookAtPoint = -1;
                    newpath.speed = 100;
                    newpath.useLookAt = false;
                    this->paths.push_back(newpath);

                }
            }
        } break;
        case VK_F6: {
            if (this->p_bIsActive) {
                this->p_bControlState = CMOD_PLAYER;
                this->playingID = 0;
                this->playingTime = 0;
            }
        } break;
        case VK_F8: {
            if (this->p_bIsActive) {
                this->paths.clear();
                this->points.clear();
            }
        } break;

        case 0x57: // W
        case VK_UP: {
            if (this->p_bControlState == CMOD_FREECAM) {
                this->p_vectCamera.x += this->p_vectCameraRot.x * 0.2f * Camspeed;
                this->p_vectCamera.y += this->p_vectCameraRot.y * 0.2f * Camspeed;
                this->p_vectCamera.z += this->p_vectCameraRot.z * 0.2f * Camspeed;
                this->UpdateCam();
            }
        } break;
        case 0x53: // S
        case VK_DOWN: {
            if (this->p_bControlState == CMOD_FREECAM) {
                this->p_vectCamera.x -= this->p_vectCameraRot.x * 0.2f * Camspeed;
                this->p_vectCamera.y -= this->p_vectCameraRot.y * 0.2f * Camspeed;
                this->p_vectCamera.z -= this->p_vectCameraRot.z * 0.2f * Camspeed;
                this->UpdateCam();
            }
        } break;
        case 0x44: // D
        case VK_RIGHT: {
            if (this->p_bControlState == CMOD_FREECAM) {
                this->p_vectCamera.z -= this->p_vectCameraRot.x * 0.2f * Camspeed;
                // this->p_vectCamera.y += this->p_vectCameraRot.y * 0.2f;
                this->p_vectCamera.x += this->p_vectCameraRot.z * 0.2f * Camspeed;
                this->UpdateCam();
            }
        } break;
        case 0x41: // D
        case VK_LEFT: {
            if (this->p_bControlState == CMOD_FREECAM) {
                this->p_vectCamera.z += this->p_vectCameraRot.x * 0.2f * Camspeed;
                // this->p_vectCamera.y -= this->p_vectCameraRot.y * 0.2f;
                this->p_vectCamera.x -= this->p_vectCameraRot.z * 0.2f * Camspeed;
                this->UpdateCam();
            }
        } break;
        case VK_PRIOR: {
            if (this->p_bControlState == CMOD_FREECAM) {
                this->p_vectCamera.y += 2.0f;
                this->UpdateCam();
            }
        } break;
        case VK_NEXT: {
            if (this->p_bControlState == CMOD_FREECAM) {
                this->p_vectCamera.y -= 2.0f;
                this->UpdateCam();
            }
        } break;
        default:
            break;
        }
    }
}

void CModControl::OnMouseMove(int x, int y)
{
    if (this->p_bIsActive) {
        if (this->p_bControlState == CMOD_FREECAM) {
            if (x != 0) {
                float getAngle = RotationTo360(this->p_vectCameraRot.x, this->p_vectCameraRot.z);
                getAngle = getAngle + ((float)x * 0.1f);

                this->p_vectCameraRot.z = cos(getAngle / RADIAN);
                this->p_vectCameraRot.x = sin(getAngle / RADIAN);

                this->UpdateCam();
            }
            if (y != 0) {
                this->p_vectCameraRot.y = sin(
                    (ASinTo180(this->p_vectCameraRot.y) - (float)y * 0.1f) / RADIAN);
            }
        }
    }
}

void CModControl::OnMouseButtons(unsigned short buttons)
{
    if (this->p_bIsActive) {
        if (this->p_bControlState == CMOD_PICKING) {
            if (buttons & RI_MOUSE_LEFT_BUTTON_DOWN) {
                Point2D mouse = core->getGraphics()->GetMouseCoords();

                int len = this->points.size();

                for (int i = 0; i < len; i++) {
                    Vector3D pos;
                    if (core->getGraphics()->Get2DFrom3D(points[i].position, &pos)) {
                        /*if ((mouse.x == (int)pos.x) && (mouse.y == (int)pos.y))
            {
            this->pickedPoint = i;
            return;
            }*/
                        if (IsInRadius2D(mouse.x, mouse.y, (int)pos.x, (int)pos.y, 10)) {
                            // this->pickedPoint = i;
                            if (GetKeyState(VK_CONTROL) & 0x8000) {
                                this->pickedPointSecond = points[i].uniqueID;
                            } else {
                                this->pickedPoint = points[i].uniqueID;
                            }
                            return;
                        }
                    }
                }
                this->pickedPoint = -1;
                this->pickedPointSecond = -1;
            }
        }
    }
}

void CModControl::ActivateFreeCam(bool state)
{
    if (state) {
        this->p_vectCamera = core->getGame()->GetPlayerPosition();
        this->p_vectCamera.y += 1.0f;
        this->p_vectCameraRot = core->getGame()->GetPlayerRotation();

        this->UpdateCam();
    } else {
        core->getGame()->CameraUnlock();
    }
    // this->p_bControlState == CMOD_FREECAM);
    // this->BlockInput(state);
}

void CModControl::UpdateCam()
{
    core->getGame()->SetCameraPos(this->p_vectCamera, this->p_vectCameraRot);
}

bool CModControl::isFreeCam()
{
    return (this->p_bControlState == CMOD_FREECAM);
}

void CModControl::BlockInput(bool should) { this->p_bBlockInput = should; }
bool CModControl::IsInputBlocked() { return this->p_bBlockInput; }

unsigned char CModControl::GetState() { return this->p_bControlState; }

void CModControl::Render()
{
    /*if (this->GetState() == CMOD_PLAYER)
  {
          *(DWORD*) 0x006613D4 |= 1;
  }
  else {
          *(DWORD*)0x006613D4 &= ~1;
  }*/
    char buff[500];
    if (!this->p_bIsActive) {
        /*// mod isn't active, player has game's control
    Vector3D position, rotation;
    position = core->getGame()->GetPlayerPosition();
    rotation = core->getGame()->GetPlayerRotation();

    sprintf(buff, "Pice: %.2f %.2f %.2f Rotation: %.2f %.2f %.2f", position.x,
    position.y, position.z, rotation.x, rotation.y, rotation.z);

    //core->getGraphics()->TextDraw(10, 10, buff, D3DCOLOR_XRGB(0, 0xad, 0xef));

    sprintf(buff, "X: %d Y:%d", core->getRawInput()->x, core->getRawInput()->y);

    //core->getGraphics()->TextDraw(10, 50, buff, D3DCOLOR_XRGB(0, 0xad, 0xef));


    Vector3D rot = core->getGame()->GetPlayerRotation();
    sprintf(buff, "Rotacia: %f %f", RotationTo360(rot.z, rot.x),
    ASinTo180(rot.y));

    //core->getGraphics()->TextDraw(10, 100, buff, D3DCOLOR_XRGB(0, 0xad,
    0xef));*/

        // core->getGraphics()->Clear(8, 8, 130, 25,0xFF000000);
        Point2D screen = core->getGraphics()->GetScreenSize();
        if (core->getGame()->IsGameRunning()) {
            core->getGraphics()->TextDraw(10, 10, "Press F1 to start", 0xFFffFFff);
        } else
            core->getGraphics()->TextDraw(10, 10, "At first run game", 0xFFffFFff);

        // core->getGraphics()->FillARGBGradient(0, 0, screen.x, screen.y,
        // 0x20FFFFFF, 0x7E000000);
    } else {
        float uhol = ASinTo180(this->p_vectCameraRot.y);

        sprintf(buff, "Rot: %f Uhol2 %f",
            RotationTo360(this->p_vectCameraRot.x, this->p_vectCameraRot.z),
            uhol);
        // core->getGraphics()->TextDraw(10, 10, buff, D3DCOLOR_XRGB(255, 255,
        // 255));

        // core->getGraphics()->TextDraw(10, 200, buff, D3DCOLOR_XRGB(0, 0xad,
        // 0xef));

        int len = this->points.size();

        if (this->GetState() != CMOD_PLAYER) {
            for (int i = 0; i < len; i++) {
                Vector3D pos;
                if (core->getGraphics()->Get2DFrom3D(points[i].position, &pos)) {
                    int size = 30 - (int)(pos.z * 20);
                    if (points[i].uniqueID == this->pickedPoint || points[i].uniqueID == this->pickedPointSecond)
                        core->getGraphics()->Clear(pos.x - size, pos.y - size, size * 2,
                            size * 2, D3DCOLOR_XRGB(0, 200, 0));
                    else
                        core->getGraphics()->Clear(pos.x - size, pos.y - size, size * 2,
                            size * 2, D3DCOLOR_XRGB(200, 0, 0));

                    sprintf(buff, "ID: %d", points[i].uniqueID);
                    core->getGraphics()->TextDraw(pos.x, pos.y - 20, buff,
                        D3DCOLOR_XRGB(255, 255, 255));
                }
            }
        }

        if (this->p_bControlState == CMOD_PICKING) {
        }

        if (this->p_bControlState == CMOD_PLAYER) {
            if (playingTime == playintSpeed) {
                this->p_bControlState = CMOD_FREECAM;
            }

            playintSpeed = paths[0].speed;
            float rotA, rotB, newRot;

            // rotA = RotationTo360(points[paths[0].pointA].rotation.x,
            // points[paths[0].pointA].rotation.z); rotB =
            // RotationTo360(points[paths[0].pointB].rotation.x,
            // points[paths[0].pointB].rotation.z);

            rotA = RotationTo360(this->GetPointID(paths[0].pointA)->rotation.x,
                this->GetPointID(paths[0].pointA)->rotation.z);
            rotB = RotationTo360(this->GetPointID(paths[0].pointB)->rotation.x,
                this->GetPointID(paths[0].pointB)->rotation.z);

            newRot = rotB - rotA;
            if (newRot > 180) {
                newRot = -(360 - rotB + rotA);
            } else if (newRot < -180) {
                newRot = (360 - rotB + rotA);
            }

            newRot = (newRot / playintSpeed) * playingTime;

            newRot += rotA;
            if (newRot < 0)
                newRot = 360 + newRot;

            float rotY = this->GetPointID(paths[0].pointB)->rotation.y - this->GetPointID(paths[0].pointA)->rotation.y;
            rotY = (rotY / playintSpeed) * playingTime;

            Vector3D newRotation;
            newRotation.x = sin(newRot * TORADIAN);
            newRotation.y = this->GetPointID(paths[0].pointA)->rotation.y + rotY;
            newRotation.z = cos(newRot * TORADIAN);

            sprintf(buff, "A: %f B: %f Rot: %f %f %f", rotA, rotB, newRotation.x,
                newRotation.y, newRotation.z);
            // core->getGraphics()->TextDraw(10, 480, buff, D3DCOLOR_XRGB(255, 255,
            // 255));

            Vector3D vectoriQ;
            vectoriQ.x = (this->GetPointID(paths[0].pointB)->position.x - this->GetPointID(paths[0].pointA)->position.x) / playintSpeed;
            vectoriQ.y = (this->GetPointID(paths[0].pointB)->position.y - this->GetPointID(paths[0].pointA)->position.y) / playintSpeed;
            vectoriQ.z = (this->GetPointID(paths[0].pointB)->position.z - this->GetPointID(paths[0].pointA)->position.z) / playintSpeed;

            sprintf(buff, "Cam: %f %f %f", vectoriQ.x, vectoriQ.y, vectoriQ.z);
            // core->getGraphics()->TextDraw(10, 500, buff, D3DCOLOR_XRGB(255, 255,
            // 255));

            vectoriQ.x *= playingTime;
            vectoriQ.y *= playingTime;
            vectoriQ.z *= playingTime;

            sprintf(buff, "Cam: %f %f %f", vectoriQ.x, vectoriQ.y, vectoriQ.z);
            // core->getGraphics()->TextDraw(10, 520, buff, D3DCOLOR_XRGB(255, 255,
            // 255));

            vectoriQ.x += this->GetPointID(paths[0].pointA)->position.x;
            vectoriQ.y += this->GetPointID(paths[0].pointA)->position.y;
            vectoriQ.z += this->GetPointID(paths[0].pointA)->position.z;
            core->getGame()->SetCameraPos(vectoriQ,
                core->getGame()->GetPlayerRotation());
            core->getGame()->SetCameraPos(vectoriQ, newRotation);

            playingTime++;

            sprintf(buff, "Cam: %f %f %f", vectoriQ.x, vectoriQ.y, vectoriQ.z);
            // core->getGraphics()->TextDraw(10, 540, buff, D3DCOLOR_XRGB(255, 255,
            // 255));
        }
        // core->getGraphics()->TextDraw(500, 400, core->getRawInput()->debuf,
        // D3DCOLOR_XRGB(255, 255, 255));
    }
}

void CModControl::ToggleMod()
{
    if (this->p_bIsActive == true) {
        this->ActivateFreeCam(false);
        core->getGame()->CameraUnlock();
    } else {
        this->ActivateFreeCam(true);
    }
    this->p_bIsActive = !this->p_bIsActive;
}

void CModControl::Init()
{
}

CamPoint* CModControl::GetPointID(int ID)
{
    int size = points.size();
    for (int i = 0; i < size; i++) {
        if (points[i].uniqueID == ID)
            return &points[i];
    }
    return NULL;
}
