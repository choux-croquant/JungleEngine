class Cube {
public:
    Cube(FVector _position, FVector _rotation, FVector _scale)
        : position(_position), rotation(_rotation), scale(_scale) {
    };

    FVector position;
    FVector rotation; // X, Y, Z ȸ�� �� (����, ����: Degree)
    FVector scale;    // X, Y, Z ũ�� ����

    void Render() {
        FConstants constantData = {};

        // Scale ���
        FMatrix scaleMatrix = FMatrix::Scale(scale);

        // Rotation ��� (�� �ະ ȸ�� ����)
        FMatrix rotationMatrix =
            FMatrix::RotateZ(rotation.Z) *
            FMatrix::RotateY(rotation.Y) *
            FMatrix::RotateX(rotation.X);

        // Translation(�̵�) ���
        FMatrix translationMatrix = FMatrix::Identity.Translate(position);

        // ModelMatrix = Scale * Rotation * Translation
        FMatrix model = translationMatrix * rotationMatrix * scaleMatrix;

        // ViewMatrix ����
        FMatrix view = FMatrix::LookAt(
            FVector(0.0f, 2.0f, -10.0f), // ī�޶� ��ġ
            FVector(0.0f, 0.0f, 0.0f),   // �ٶ󺸴� ����
            FVector(0.0f, 1.0f, 0.0f)    // �� ����
        );

        // ProjectionMatrix ����
        FMatrix projection = FMatrix::Perspective(
            0.785398f,       // FOV (45��)
            (float)1024 / 1024, // ȭ�� ����
            1.0f,            // ����� Ŭ���� ���
            100.0f           // �� Ŭ���� ���
        );

        // MVP ��� ���
        FMatrix mvp = projection * view * model;
        constantData.MVP = mvp;

        // ������ ������ ������Ʈ �� �׸���
        URenderer::GetInstance().UpdateConstant(constantData);
        URenderer::GetInstance().RenderPrimitive();
    }
};
